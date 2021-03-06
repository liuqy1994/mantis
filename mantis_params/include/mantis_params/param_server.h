/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <ros/ros.h>
#include <dh_parameters/JointDescription.h>
#include <mantis_msgs/BodyInertial.h>
#include <mantis_msgs/Parameters.h>
#include <std_srvs/Empty.h>

#include <string>
#include <vector>

namespace MantisParams {
class Server {
	private:
		ros::NodeHandle nh_;
		ros::Publisher pub_params_;

		ros::ServiceServer srv_reload_;

		ros::Time load_time_;

		std::string model_id_;
		std::string airframe_name_;
		int pwm_min_;
		int pwm_max_;

		double la_;
		double cant_;
		//int motor_num_;
		//double motor_kv_;
		//double rpm_thrust_m_;
		//double rpm_thrust_c_;
		double motor_thrust_max_;
		double motor_drag_max_;

		int body_num_;
		std::vector<mantis_msgs::BodyInertial> bodies_;
		std::vector<dh_parameters::JointDescription> joints_;

	public:
		Server( void );

		~Server( void );

		void update( void );
		mantis_msgs::Parameters get_params( void );

		bool ok( void );

	private:
		void load( void );
		bool reload(std_srvs::Empty::Request  &req, std_srvs::Empty::Response &res);
};
};
