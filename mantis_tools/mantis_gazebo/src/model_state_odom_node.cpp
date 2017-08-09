#include <ros/ros.h>

#include <tf2/LinearMath/Transform.h>
#include <tf2/LinearMath/Vector3.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <nav_msgs/Odometry.h>
#include <gazebo_msgs/ModelStates.h>

#include <string>

class ModelStatesOdom {
	private:
		ros::NodeHandle nh_;

		ros::Subscriber sub_model_states_;
		ros::Publisher pub_odom_;

		std::string topic_model_states_;
		std::string topic_odom_;

		std::string parent_name_;
		std::string model_name_;
		nav_msgs::Odometry msg_odom_;

	public:
		ModelStatesOdom() :
			nh_( "~" ),
			model_name_("mantis_uav"),
			parent_name_("world"),
			topic_model_states_("/gazebo/model_states"),
			topic_odom_("/mantis_uav/odom") {

			sub_model_states_ = nh_.subscribe<gazebo_msgs::ModelStates>(topic_model_states_, 1, &ModelStatesOdom::statesCallback, this);
			pub_odom_ = nh_.advertise<nav_msgs::Odometry>( topic_odom_, 10 );

			msg_odom_.header.frame_id = parent_name_;
			msg_odom_.child_frame_id = model_name_;

			ROS_INFO("Listenning for model states...");
		}

		~ModelStatesOdom() {
			//This message won't actually send here, as the node will have already shut down
			ROS_INFO("Shutting down...");
		}

		void statesCallback(const gazebo_msgs::ModelStates::ConstPtr& msg_in) {
			msg_odom_.header.stamp = ros::Time::now();

			for( int i=0; i<msg_in->name.size(); i++ ) {
				if( msg_in->name[i] == model_name_ ) {
					tf2::Quaternion tmp_q( msg_in->pose[i].orientation.x,
										   msg_in->pose[i].orientation.y,
										   msg_in->pose[i].orientation.z,
										   msg_in->pose[i].orientation.w );

					tf2::Quaternion rot_vel;
					rot_vel = ( tmp_q * tf2::Quaternion( msg_in->twist[i].linear.x,
														 msg_in->twist[i].linear.y,
														 msg_in->twist[i].linear.z,
														 0.0 ) ) * tmp_q.inverse();

					msg_odom_.pose.pose = msg_in->pose[i];
					msg_odom_.twist.twist = msg_in->twist[i];
					/*
					msg_odom_.twist.linear.x = rot_vel.getX();
					msg_odom_.twist.linear.y = rot_vel.getY();
					msg_odom_.twist.linear.z = rot_vel.getZ();
					*/

					msg_odom_.header.stamp = ros::Time::now();
					pub_odom_.publish(msg_odom_);

					break;
				}
			}
		}
};

int main(int argc, char** argv) {
	ros::init(argc, argv, "model_states_odom");
	ModelStatesOdom mso;

	ros::spin();

	return 0;
}
