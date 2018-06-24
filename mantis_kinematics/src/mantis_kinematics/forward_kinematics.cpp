#include <ros/ros.h>

#include <mantis_kinematics/forward_kinematics.h>
#include <mantis_description/se_tools.h>

#include <tf2_ros/transform_listener.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>

#include <tf2/LinearMath/Vector3.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_eigen/tf2_eigen.h>

#include <eigen3/Eigen/Dense>

#include <string>
#include <vector>

#include <stdio.h>

//TODO: Add goal joints

ForwardKinematics::ForwardKinematics() :
	nh_(),
	nhp_("~"),
	p_(&nh_),
	s_(&nh_),
	solver_(&p_, &s_),
	param_frame_id_("map"),
	param_model_id_("mantis_uav"),
	param_rate_(30.0),
	param_do_end_effector_pose_(true),
	param_do_viz_(true),
	param_done_viz_(false),
	tfBuffer_(ros::Duration(20.0)) {

	//Load parameters
	nhp_.param("frame_id", param_frame_id_, param_frame_id_);
	nhp_.param("model_id", param_model_id_, param_model_id_);
	nhp_.param("update_rate", param_rate_, param_rate_);
	nhp_.param("do_viz", param_do_viz_, param_do_viz_);
	nhp_.param("end_effector_pose", param_do_end_effector_pose_, param_do_end_effector_pose_);

	if( p_.wait_for_params() ) {
		//Configure publishers and subscribers
		pub_end_ = nh_.advertise<geometry_msgs::PoseStamped>("pose/end_effector", 10);
		pub_viz_ = nh_.advertise<visualization_msgs::MarkerArray>("visualization", 10, true);

		ROS_INFO("Configuring static mounts...");
		do_reset();

		timer_ = nhp_.createTimer(ros::Duration(1.0/param_rate_), &ForwardKinematics::callback_timer, this );

		ROS_INFO("Forward kinematics running!");
	} else {
		ros::shutdown();
	}
}

ForwardKinematics::~ForwardKinematics() {
}

void ForwardKinematics::callback_timer(const ros::TimerEvent& e) {
	check_update_time();

	geometry_msgs::TransformStamped t;

	t.header.frame_id = param_frame_id_;
	t.header.stamp = e.current_real;
	t.child_frame_id = param_model_id_;

	geometry_msgs::Pose p = pose_from_eig(s_.g());
	t.transform.translation.x = p.position.x;
	t.transform.translation.y = p.position.y;
	t.transform.translation.z = p.position.z;
	t.transform.rotation = p.orientation;

	tfbr_.sendTransform(t);
	tfBuffer_.setTransform(t, ros::this_node::getName());	//Set the base link

	for(int i=0; i<p_.get_joint_num(); i++) {
		if( p_.joint(i).type != "static" ) {
			Eigen::Affine3d g;
			if(solver_.calculate_gxy(g,i,i+1)) {
				geometry_msgs::TransformStamped transform = tf2::eigenToTransform(g);
				transform.header.stamp = e.current_real;
				transform.header.frame_id = (i == 0) ? param_model_id_ : param_model_id_ + "/link_" + std::to_string(i);
				transform.child_frame_id = param_model_id_ + "/link_" + std::to_string(i+1);

				tfbr_.sendTransform(transform);
				tfBuffer_.setTransform(transform, ros::this_node::getName());	//Set the internal dynamic joints
			}
		}
	}
}

/*
void ForwardKinematics::callback_state_odom(const nav_msgs::Odometry::ConstPtr& msg_in) {

}

void ForwardKinematics::callback_state_joints(const sensor_msgs::JointState::ConstPtr& msg_in) {
	check_update_time();

	//Send out the end effector pose
	if(param_do_end_effector_pose_) {
		geometry_msgs::PoseStamped msg_end_out_;
	    geometry_msgs::TransformStamped tf_end;
		std::string last_joint = param_model_id_ + "/link_" + std::to_string(param_joints_.size());

		try {
			tf_end = tfBuffer_.lookupTransform(param_frame_id_, last_joint, ros::Time(0));

			msg_end_out_.header = tf_end.header;
			msg_end_out_.pose.position.x = tf_end.transform.translation.x;
			msg_end_out_.pose.position.y = tf_end.transform.translation.y;
			msg_end_out_.pose.position.z = tf_end.transform.translation.z;
			msg_end_out_.pose.orientation.w = tf_end.transform.rotation.w;
			msg_end_out_.pose.orientation.x = tf_end.transform.rotation.x;
			msg_end_out_.pose.orientation.y = tf_end.transform.rotation.y;
			msg_end_out_.pose.orientation.z = tf_end.transform.rotation.z;

			pub_end_.publish(msg_end_out_);
		}
		catch(const tf2::LookupException& e) {
			ROS_WARN_THROTTLE(1.0, "Lookup error: %s", e.what());
		}
		catch(const tf2::ExtrapolationException& e) {
			ROS_WARN_THROTTLE(1.0, "Extrapolation error: %s", e.what());
		}
	}
	//if(param_do_viz_ && !param_done_viz_)
	//	do_viz(&(msg_in->name));
}
*/

void ForwardKinematics::configure_static_joints() {
	ros::Time stamp = ros::Time::now();

	for(int i=0; i<p_.get_joint_num(); i++) {
		if( p_.joint(i).type == "static" ) {
			Eigen::Affine3d g;
			if(solver_.calculate_gxy(g,i,i+1)) {
				geometry_msgs::TransformStamped tf = tf2::eigenToTransform(g);
				tf.header.stamp = stamp;
				tf.header.frame_id = (i == 0) ? param_model_id_ : param_model_id_ + "/link_" + std::to_string(i);
				tf.child_frame_id = param_model_id_ + "/link_" + std::to_string(i+1);

				tfsbr_.sendTransform(tf);
				tfBuffer_.setTransform(tf, ros::this_node::getName(), true);	//Set the internal static joints
			}
		}
	}
}

void ForwardKinematics::do_reset() {
	tfBuffer_.clear();

	configure_static_joints();

	time_last_update_ = ros::Time::now();
}

void ForwardKinematics::check_update_time() {
	if(ros::Time::now() < time_last_update_) {
		ROS_INFO("Detected jump back in time, reseting transforms");
		do_reset();
	} else {
		time_last_update_ = ros::Time::now();
	}
}


void ForwardKinematics::do_viz( const std::vector<std::string> *arm_names ) {
	/*
	visualization_msgs::MarkerArray markers;
	ros::Time stamp = ros::Time::now();

	visualization_msgs::Marker m;
	m.header.stamp = stamp;
	m.ns = param_model_name_;
	m.action = m.ADD;
	m.color.a = 1.0;
	m.lifetime = ros::Duration(0);
	m.frame_locked = true;

	//Body
	//TODO: params
	double frame_h = 0.05;
	double frame_w = 0.55;

	m.header.frame_id = param_model_name_;
	m.id = 0;
	m.type = m.CYLINDER;
	m.pose.position.x = 0.0;
	m.pose.position.y = 0.0;
	m.pose.position.z = frame_h/2;
	m.pose.orientation.w = 1.0;
	m.pose.orientation.x = 0.0;
	m.pose.orientation.y = 0.0;
	m.pose.orientation.z = 0.0;
	m.scale.x = frame_w;
	m.scale.y = frame_w;
	m.scale.z = frame_h;
	m.color.r = 0.8;
	m.color.g = 0.8;
	m.color.b = 0.8;

	markers.markers.push_back(m);

	//Arms
	for(int i=0; i<arm_names->size(); i++) {
		double arm_w = 0.05;

		m.header.frame_id = (*arm_names)[i];
		m.id++;
		m.type = m.CYLINDER;
		m.pose.position.x = param_arm_len_/2;
		m.pose.position.y = 0.0;
		m.pose.position.z = 0.0;
		m.pose.orientation.w = 0.7071;
		m.pose.orientation.x = 0.0;
		m.pose.orientation.y = 0.7071;
		m.pose.orientation.z = 0.0;
		m.scale.x = arm_w;
		m.scale.y = arm_w;
		m.scale.z = param_arm_len_;
		m.color.r = 0.8;
		m.color.g = 0.8;
		m.color.b = 0.8;

		markers.markers.push_back(m);
	}

	pub_viz_.publish(markers);
	*/
	param_done_viz_ = true;
}