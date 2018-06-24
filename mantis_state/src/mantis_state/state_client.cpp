#include <eigen3/Eigen/Dense>

#include <ros/ros.h>
#include <mantis_msgs/State.h>
#include <mantis_description/se_tools.h>
#include <mantis_state/state_client.h>

MantisStateClient::MantisStateClient( ros::NodeHandle *nh ) :
	nh_(nh),
	timestamp_(0),
	voltage_(0.0),
	flight_ready_(false) {

	g_ = Eigen::Affine3d::Identity();
	bv_ = Eigen::Vector3d::Zero();
	ba_ = Eigen::Vector3d::Zero();
	bw_ = Eigen::Vector3d::Zero();
	bwa_ = Eigen::Vector3d::Zero();

	sub_state_ = nh_->subscribe<mantis_msgs::State>( "state", 1, &MantisStateClient::callback_state, this );
}

MantisStateClient::~MantisStateClient() {
}

bool MantisStateClient::ok( void ) {
	return ( timestamp_ != ros::Time(0) );
}

const ros::Time& MantisStateClient::time_updated( void ) {
	return timestamp_;
}

const Eigen::Affine3d& MantisStateClient::g( void ) {
	return g_;
}

const Eigen::Vector3d& MantisStateClient::bv( void ) {
	return bv_;
}

Eigen::Vector3d MantisStateClient::wv( void ) {
	return g_.linear()*bv_;
}

const Eigen::Vector3d& MantisStateClient::bw( void ) {
	return bw_;
}

const Eigen::Vector3d& MantisStateClient::ba( void ) {
	return ba_;
}

const Eigen::Vector3d& MantisStateClient::bwa( void ) {
	return bwa_;
}

const Eigen::VectorXd& MantisStateClient::r( void ) {
	return r_;
}

const Eigen::VectorXd& MantisStateClient::rd( void ) {
	return rd_;
}

const Eigen::VectorXd& MantisStateClient::rdd( void ) {
	return rdd_;
}

const double& MantisStateClient::voltage( void ) {
	return voltage_;
}

const bool& MantisStateClient::flight_ready( void ) {
	return flight_ready_;
}

void MantisStateClient::callback_state(const mantis_msgs::State::ConstPtr &msg_in) {
	timestamp_ = msg_in->header.stamp;

	g_ = affine_from_msg(msg_in->pose);
	bv_ = vector_from_msg(msg_in->twist.linear);
	bw_ = vector_from_msg(msg_in->twist.angular);
	ba_ = vector_from_msg(msg_in->accel.linear);
	bwa_ = vector_from_msg(msg_in->accel.angular);

	int num_manip_states = msg_in->r.size();
	if(r_.size() != num_manip_states) {
		//Resize our vectors if the state changed
		r_ = Eigen::VectorXd::Zero(num_manip_states);
		rd_ = Eigen::VectorXd::Zero(num_manip_states);
		rdd_ = Eigen::VectorXd::Zero(num_manip_states);
	}

	for(int i=0; i<num_manip_states; i++) {
		r_[i] = msg_in->r[i];
		rd_[i] = msg_in->rd[i];
		rdd_[i] = msg_in->rdd[i];
	}

	voltage_ = msg_in->battery_voltage;
	flight_ready_ = msg_in->flight_ready;
}