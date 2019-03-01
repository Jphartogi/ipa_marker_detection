#include "ros/ros.h"
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include "fiducial_msgs/FiducialTransformArray.h"
#include <geometry_msgs/Transform.h>
#include <std_msgs/Header.h>
#include <geometry_msgs/Twist.h>

class LocationFinder{
public:
	void Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg){
		/// check if the message is empty
		if (msg->transforms.empty())
		{
			std_msgs::Header header;
			double time = msg->header.stamp.toSec(); // get the time from message
			// ROS_INFO("No Marker Detected!");
			static tf::TransformBroadcaster br;
			tf::Transform trf2;
			trf2.setOrigin( tf::Vector3(0,0,0));
			trf2.setRotation(tf::Quaternion(0,0,0,1)); // set transform to 0 but not publishing!

			// br.sendTransform(tf::StampedTransform(trf2,ros::Time(time),"camera_color_optical_frame","marker_pose"));
			marker_detected_ = false;
		}
		else{
		/// when the marker is detected

			///obtain the pose
			geometry_msgs::Transform transform;
			double transform_x = msg->transforms[0].transform.translation.x;
			double transform_y = msg->transforms[0].transform.translation.y;
			double transform_z = msg->transforms[0].transform.translation.z;
			// ROS_INFO("x: %f,y: %f,z: %f",transform_x,transform_y,transform_z);

			double rotation_x = msg->transforms[0].transform.rotation.x;
			double rotation_y = msg->transforms[0].transform.rotation.y;
			double rotation_z = msg->transforms[0].transform.rotation.z;
			double rotation_w = msg->transforms[0].transform.rotation.w;
			// ROS_INFO("x: %f,y: %f,z: %f,w: %f",rotation_x,rotation_y,rotation_z,rotation_w);
			// obtain the time
			std_msgs::Header header;
		 	time = msg->header.stamp.toSec();

			tf::TransformBroadcaster br;
			tf::Transform transform_base_camera;
			transform_base_camera.setOrigin(tf::Vector3(transform_x,transform_y,transform_z));
			transform_base_camera.setRotation(tf::Quaternion(rotation_x,rotation_y,rotation_z,rotation_w));

			// transform_base_camera.setRotation(tf::Quaternion(rotation_x,rotation_y,rotation_z,rotation_w));
			br.sendTransform(tf::StampedTransform(transform_base_camera,ros::Time(time),"camera_color_optical_frame","marker_pose"));

			ros::Rate rate(1000);
			// rate.sleep();
			marker_detected_ = true;

		}
	}

void Callback_2DOF(const geometry_msgs::PoseStamped::Ptr& msg){
	// int transform[] = {};

		/// check if no marker has detected yet
		if (msg->pose.position.x < 0.1)
		{
			marker_detected_ = false;
			static tf::TransformBroadcaster br;
			tf::Transform trf2;
			trf2.setOrigin( tf::Vector3(0,0,0));
			trf2.setRotation(tf::Quaternion(0,0,0,1));   // set transform to 0 but not publishing!
			// br.sendTransform(tf::StampedTransform(trf2,ros::Time(time),"base_link","station_charger"));

		}
		else{
		/// when the marker is detected

			///obtain the pose
			geometry_msgs::Pose pose;
			double transform_x = msg->pose.position.x;
			double transform_y = msg->pose.position.y;

			double yaw= tf::getYaw(msg->pose.orientation);

			tf::TransformBroadcaster br;
			tf::Transform transform_base_marker;
			transform_base_marker.setOrigin( tf::Vector3(transform_x,transform_y, 0));
			tf::Quaternion w;
			w.setRPY(0,0,yaw);  // set the 2DOF orientation in Roll Pitch and Yaw. The orientation needed is only the yaw.
			transform_base_marker.setRotation(w);
		// transform_base_camera.setRotation(tf::Quaternion(rotation_x,rotation_y,rotation_z,rotation_w));
			br.sendTransform(tf::StampedTransform(transform_base_marker,ros::Time(time),"base_link","station_charger"));
			ros::Rate rate(1000);
			// rate.sleep();
			marker_detected_ = true;
			// Drive(transform_x,0);																		/// to drive or send velocity to robot
			// detectAngle(transform_x,transform_y,yaw);							/// detect the orientation of the robot and compare it with the orientation from the marker

			// std::cout << "the distance x is " << transform_x << '\n';
	}

}
// 
// 	void Drive(double x,double z){
// 		pub = n.advertise<geometry_msgs::Twist>("cmd_vel",10);
//
// 		double speed_x = 0.1 * x; double speed_y = 0.1;
// 		double speed = 0.01;
// 		if (x > 0.3) {
// 			go = true;
// 			std::cout << "it's true!!" << '\n';
// 			velocity.linear.x = speed_x;
// 			velocity.linear.y = 0 * speed_y;
// 			velocity.linear.z = 0 * speed;
//
// 			velocity.angular.x = 0;
// 			velocity.angular.y = 0;
// 			velocity.angular.z = z;
// 			}
// 		else{
// 			go = false;
// 			velocity.linear.x = 0.0001;
// 			velocity.linear.y = 0.0001 * speed;
// 			velocity.linear.z = 0 * speed;
//
// 			velocity.angular.x = 0;
// 			velocity.angular.y = 0;
// 			velocity.angular.z = z;
// 		}
//
// 		std::cout << velocity << std::endl;
//    	// pub.publish(velocity);
//
//
// 	}
//
// 	void detectAngle(double x, double y, double yaaw)
// 	{
//
// 		tf::StampedTransform robot_transform;
// 		geometry_msgs::PoseStamped rp;
// 		try{
// 			  listener.lookupTransform("/odom","/base_link",ros::Time(0),robot_transform);
// 			 }
// 		catch(tf::TransformException& ex)
// 			 {
// 			 	ROS_ERROR("%s",ex.what());
// 			 	ros::Duration(0.1).sleep();
//
// 			 }
// 			         // echo_transform.getBasis().getRPY(roll, pitch, yaw);
// 			         tf::Quaternion qt = robot_transform.getRotation();
// 			         tf::Vector3 vt = robot_transform.getOrigin();
// 			         geometry_msgs::Pose pose;
// 			         // fiducial_msgs::FiducialTransform ft;
// 			         rp.pose.position.x = vt.getX();
// 			         rp.pose.position.y = vt.getY();
// 			         rp.pose.position.z = 0;
//
// 							 double yaw= tf::getYaw(qt);
// 							 std::cout << "yawnya robot " << yaw << '\n';
// 							 std::cout << "xnya robot " << rp.pose.position.x << '\n';
// 							 std::cout << "ynya robot" << rp.pose.position.y << '\n';
// 							 if (yaw == yaaw) {
// 							 	allow_rotation_ == false;
// 									Drive(0,0);
// 							 }
// 							 else {
// 								 allow_rotation_ == true;
// 								 	Drive(0,0.5);
// 							 }
//
// 	}


private:
		double time;
		tf::Transform transform_;
		bool marker_detected_;
		bool allow_rotation_;
		bool go;
		ros::NodeHandle n;
		ros::Publisher pub;
		tf::TransformListener listener;
		geometry_msgs::Twist velocity;


};

int main(int argc,char *argv[]){
	ros::init(argc,argv,"location_finder");
	ros::NodeHandle nh;
	LocationFinder location;

	ros::Subscriber sub = nh.subscribe("fiducial_transforms",10,&LocationFinder::Callback,&location); //subscribe to a publish message from fiducial transforms which is published from the aruco_detect
	ros::Subscriber sub2 = nh.subscribe("Position",10,&LocationFinder::Callback_2DOF,&location); // subscribe to a publish message from a Position , that is published in location_listener.cpp

  ros::spin();
	return 0;
}
