/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <dynamic_reconfigure/server.h>
#include <ros/ros.h>

#include <mantis_controller_feed/ControlParamsConfig.h>
#include <mantis_controller_feed/controller_feed.h>
#include <mantis_description/se_tools.h>

#include <geometry_msgs/WrenchStamped.h>
#include <mavros_msgs/AttitudeTarget.h>

#include <eigen3/Eigen/Dense>
#include <iostream>
#include <math.h>

#define CONST_GRAV 9.80665
#define CONST_ACTC_ADD 1

ControllerFeed::ControllerFeed()
	: nh_()
	, nhp_( "~" )
	, p_( nh_ )
	, s_( nh_, p_ )
	, solver_( p_, s_ )
	, param_est_rate_( 50.0 )
	, cmd_throttle_( 0.0 )
	, dyncfg_control_settings_( ros::NodeHandle( nhp_, "control_settings" ) ) {

	bool success = true;

	nhp_.param( "estimator_rate", param_est_rate_, param_est_rate_ );

	dyncfg_control_settings_.setCallback( boost::bind(
		&ControllerFeed::callback_cfg_control_settings, this, _1, _2 ) );

	// Wait here for parameters to be loaded
	if ( p_.wait_for_params() && s_.wait_for_state() ) {
		pub_np_force_ = nhp_.advertise<mavros_msgs::ActuatorControl>(
			"output/normalized_payload_torque", 10 );
		pub_wrench_ = nhp_.advertise<geometry_msgs::WrenchStamped>(
			"feedback/wrench_compensation", 10 );

		sub_state_attitude_target_ = nhp_.subscribe<mavros_msgs::AttitudeTarget>(
			"command/attitude", 10, &ControllerFeed::callback_state_attitude_target,
			this );

		ROS_INFO( "Augmented dynamics controller loaded. Waiting for inputs:" );
		ROS_INFO( "    - attitude target" );
		ROS_INFO( "    - state" );

		// Lock the controller until all the inputs are satisfied
		while ( ros::ok() && ( msg_attitude_target_tr_ == ros::Time( 0 ) ) ) {

			if ( msg_attitude_target_tr_ != ros::Time( 0 ) )
				ROS_INFO_ONCE( "Augmented dynamics got input: attitude target" );

			ros::spinOnce();
			ros::Rate( param_est_rate_ ).sleep();
		}

		ROS_INFO_ONCE( "Augmented dynamics got all inputs!" );

		// Start the control loops
		timer_est_ = nhp_.createTimer( ros::Duration( 1.0 / param_est_rate_ ),
			&ControllerFeed::callback_est, this );

		ROS_INFO( "Augmented dynamics estimator started!" );
	} else {
		ROS_WARN( "Augmented Dynamics estimator shutting down." );
		ros::shutdown();
	}
}

ControllerFeed::~ControllerFeed() {
}

void ControllerFeed::callback_cfg_control_settings(
	mantis_controller_feed::ControlParamsConfig& config, uint32_t level ) {
	param_force_compensation_ = config.force_compensation;
	param_force_comp_alpha_ = config.force_comp_filter_a;
	param_coriolis_compensation_ = config.coriolis_compensation;
	param_reference_feedback_ = config.reference_feedback;
}

void ControllerFeed::callback_est( const ros::TimerEvent& e ) {
	double dt = ( e.current_real - e.last_real ).toSec();

	bool solved = false;

	//==-- Calculate forces induced by the arm
	Eigen::VectorXd tau = Eigen::VectorXd::Zero( solver_.num_states() );
	Eigen::VectorXd uaug_out;

	if ( param_force_compensation_ ) {
		double kT = 0.0;
		double ktx = 0.0;
		double kty = 0.0;
		double ktz = 0.0;

		if ( solver_.calculate_thrust_coeffs( kT, ktx, kty, ktz ) ) {
			double accel_z = cmd_throttle_ / ( kT * p_.get(MantisParams::PARAM_TOTAL_MASS) );
			// Use this to account for acceleration / translational forces
			//  but assume that all rotations want to maintain 0 acceleration

			Eigen::VectorXd ua = Eigen::VectorXd::Zero( solver_.num_states() ); // vd(6,1) + rdd(2,1)
			// ua(0) = 0.0;
			// ua(1) = 0.0;
			ua( 2 ) = 9.80665; // accel_z;
			// ua.segment(3,3) << Eigen::Vector3d::Zero();
			// ua.segment(6,p_.get_dynamic_joint_num()) << s_.rdd();

			solved = solver_.solve_inverse_dynamics( tau, ua );

			if ( solved ) {
				/*
        //Normalize and add in the augmentation force calcs
        Eigen::VectorXd uaug(3);
        uaug(0) = tau(3)*ktx;//roll accel compensation
        uaug(1) = tau(4)*kty;//pitch accel compensation
        uaug(2) = tau(5)*ktz;//yaw accel compensation
        */
				Eigen::Vector4d cforces;
				cforces << 0.0, tau( 3 ) * ktx, tau( 4 ) * kty, tau( 5 ) * ktz;
				Eigen::VectorXd uaug = p_.get(MantisParams::PARAM_MIXER) * cforces;

				if ( uaug_f_.size() != uaug.size() ) {
					uaug_f_ = Eigen::VectorXd::Zero( uaug.size() );
				}
				// Low level filter to help reduce issues caused by low level
				// oscilations in the other controllers
				uaug_f_ = param_force_comp_alpha_ * uaug + ( 1.0 - param_force_comp_alpha_ ) * uaug_f_;

				uaug_out = uaug_f_;
			} else {
				ROS_ERROR_THROTTLE( 2.0, "Unable to solve inverse dynamics!" );
			}
		} else {
			ROS_ERROR_THROTTLE( 2.0, "Unable to calculate thrust coefficients!" );
		}
	}

	if ( param_reference_feedback_ ) {
		geometry_msgs::WrenchStamped msg_wrench_out;
		msg_wrench_out.header.stamp = e.current_real;
		msg_wrench_out.header.frame_id = s_.model_id();

		if ( solved ) {
			msg_wrench_out.wrench.force.x = tau( 0 );
			msg_wrench_out.wrench.force.y = tau( 1 );
			msg_wrench_out.wrench.force.z = tau( 2 );
			msg_wrench_out.wrench.torque.x = tau( 3 );
			msg_wrench_out.wrench.torque.y = tau( 4 );
			msg_wrench_out.wrench.torque.z = tau( 5 );
		}

		pub_wrench_.publish( msg_wrench_out );
	}

	mavros_msgs::ActuatorControl msg_np_force_out;
	msg_np_force_out.header.stamp = e.current_real;
	msg_np_force_out.header.frame_id = s_.model_id();

	// Set the message to be a force input only message in the body frame
	msg_np_force_out.group_mix = CONST_ACTC_ADD;

	// Put in the force input
	if ( solved ) {
		for ( int i = 0; i < uaug_out.size(); i++ )
			msg_np_force_out.controls[i] = uaug_out( i );
	}

	// Publish!
	pub_np_force_.publish( msg_np_force_out );
}

void ControllerFeed::callback_state_attitude_target(
	const mavros_msgs::AttitudeTarget::ConstPtr& msg_in ) {
	msg_attitude_target_tr_ = msg_in->header.stamp;

	cmd_throttle_ = msg_in->thrust;
}
