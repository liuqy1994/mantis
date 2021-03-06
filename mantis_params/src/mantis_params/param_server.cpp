/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <ros/ros.h>

#include <dh_parameters/JointDescription.h>
#include <mantis_msgs/BodyInertial.h>
#include <mantis_msgs/Parameters.h>

#include <mantis_params/param_server.h>

namespace MantisParams {
Server::Server( void )
	: nh_()
	, model_id_("mantis_uav")
	, pwm_min_( 1000 )
	, pwm_max_( 2000 )
	// motor_kv_(0.0),
	// rpm_thrust_m_(0.0),
	// rpm_thrust_c_(0.0),
	, motor_thrust_max_( 0.0 )
	, motor_drag_max_( 0.0 )
	, la_( 0.0 )
	, cant_( 0.0 )
	, body_num_( 1 ) {

	bool ready = false;

	// Wait for ros time before initializing the servers
	while ( ros::ok() && ( !ready ) ) {
		ready = ros::Time::now() != ros::Time( 0 );
		ros::spinOnce();
		ros::Rate( 5 ).sleep();
	}

	if ( ready ) {
		pub_params_ = nh_.advertise<mantis_msgs::Parameters>( "params", 1, true );
		srv_reload_ = nh_.advertiseService( "reload_params", &Server::reload, this );
		std::string ns = ros::this_node::getNamespace();
		std::size_t it_name = ns.find_last_of("/");
		model_id_ = ns.substr(it_name+1);
		//ros::param::param<std::string>("model_id", model_id_, model_id_);

		update();

		ROS_INFO( "Mantis param server started!" );
	}
}

Server::~Server() {
}

bool Server::ok( void ) {
	return ( load_time_ != ros::Time( 0 ) );
}

void Server::update( void ) {
	load();

	pub_params_.publish( get_params() );
}

mantis_msgs::Parameters Server::get_params( void ) {
	mantis_msgs::Parameters p;

	p.header.stamp = load_time_;
	p.header.frame_id = model_id_;

	p.airframe_name = airframe_name_;
	p.pwm_min = pwm_min_;
	p.pwm_max = pwm_max_;
	p.base_arm_length = la_;
	p.base_motor_cant = cant_;
	// p.motor_kv = motor_kv_;
	// p.rpm_thrust_m = rpm_thrust_m_;
	// p.rpm_thrust_c = rpm_thrust_c_;
	p.motor_thrust_max = motor_thrust_max_;
	p.motor_drag_max = motor_drag_max_;

	p.bodies = bodies_;
	p.joints = joints_;

	return p;
};

bool Server::reload( std_srvs::Empty::Request& req,
	std_srvs::Empty::Response& res ) {
	update();

	return true;
}

void Server::load( void ) {
	load_time_ = ros::Time::now();

	ROS_INFO( "--== Loading Mantis Parameters ==--" );

	nh_.param( "motor/arm_len", la_, la_ );
	nh_.param( "motor/cant", cant_, cant_ );
	// nh_.param("motor/kv", motor_kv_, motor_kv_);
	// nh_.param("motor/rpm_thrust_curve/m", rpm_thrust_m_, rpm_thrust_m_);
	// nh_.param("motor/rpm_thrust_curve/c", rpm_thrust_c_, rpm_thrust_c_);
	nh_.param( "motor/thrust_max", motor_thrust_max_, motor_thrust_max_ );
	nh_.param( "motor/drag_max", motor_drag_max_, motor_drag_max_ );

	nh_.param( "airframe_name", airframe_name_, airframe_name_ );

	nh_.param( "pwm/min", pwm_min_, pwm_min_ );
	nh_.param( "pwm/max", pwm_max_, pwm_max_ );

	// Prepare vectors
	bodies_.clear();
	joints_.clear();

	// Body definitions
	nh_.param( "body/num", body_num_, body_num_ );
	for ( int i = 0; i < body_num_; i++ ) {
		mantis_msgs::BodyInertial bi;
		std::string bp = "body/b" + std::to_string( i ) + "/";

		if ( nh_.getParam( bp + "name", bi.name ) &&
			 nh_.getParam( bp + "mass/m", bi.mass ) ) {

			nh_.getParam( bp + "mass/Ixx", bi.Ixx );
			nh_.getParam( bp + "mass/Ixy", bi.Ixy );
			nh_.getParam( bp + "mass/Ixz", bi.Ixz );
			nh_.getParam( bp + "mass/Iyy", bi.Iyy );
			nh_.getParam( bp + "mass/Iyz", bi.Iyz );
			nh_.getParam( bp + "mass/Izz", bi.Izz );
			nh_.getParam( bp + "mass/com", bi.com );

			bodies_.push_back( bi );
		} else {
			// Could not find any more valid links defined, give up
			break;
		}
	}

	// Load in the link definitions
	for ( int i = 0; i < body_num_; i++ ) {
		dh_parameters::JointDescription jd;
		std::string bp = "body/b" + std::to_string( i ) + "/";

		if ( nh_.getParam( bp + "link/type", jd.type ) ) {
			 nh_.getParam( bp + "link/name", jd.name );

			nh_.getParam( bp + "link/d", jd.d );
			nh_.getParam( bp + "link/t", jd.t );
			nh_.getParam( bp + "link/r", jd.r );
			nh_.getParam( bp + "link/a", jd.a );

			nh_.getParam( bp + "link/q", jd.q );
			nh_.getParam( bp + "link/beta", jd.beta );

			joints_.push_back( jd );
		} else {
			// Could not find any more valid links defined, give up
			break;
		}
	}

	// Output to terminal some information about the loaded paramters
	ROS_INFO( "motor:" );
	ROS_INFO( "  len: %0.4f", la_ );
	// ROS_INFO("  kv: %0.4f", motor_kv_);
	// ROS_INFO("  T = %0.4fxRPM + %0.4f", rpm_thrust_m_, rpm_thrust_c_);
	ROS_INFO( "  Tmax = %0.4f", motor_thrust_max_ );
	ROS_INFO( "  Dmax = %0.4f", motor_drag_max_ );

	ROS_INFO( "airframe_name: %s", airframe_name_.c_str() );
	if ( ( airframe_name_ != "quad_x4" ) &&
		 ( airframe_name_ != "quad_p4" ) &&
		 ( airframe_name_ != "hex_x6" ) &&
		 ( airframe_name_ != "hex_x6" ) &&
		 ( airframe_name_ != "hex_fa" ) &&
		 ( airframe_name_ != "octo_x8" ) &&
		 ( airframe_name_ != "octo_p8" ) ) {

		ROS_WARN( "Specified airframe type may not be supported!" );
	}

	ROS_INFO( "pwm: [%i, %i]", pwm_min_, pwm_max_ );

	ROS_INFO_STREAM( "bodies: " << body_num_ );
	ROS_INFO_STREAM( "  inertials: " << bodies_.size() );
	ROS_INFO_STREAM( "  joints: " << joints_.size() );

	if ( ( bodies_.size() != body_num_ ) || ( joints_.size() != body_num_ ) )
		ROS_WARN( "Number of loaded joints and bodies do not match, some params may be invalid" );

	ROS_INFO( "--== Mantis Parameters Loaded ==--" );
}
};
