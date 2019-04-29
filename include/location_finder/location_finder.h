#ifndef LOCATION_FINDER_H_
#define LOCATION_FINDER_H_

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <fiducial_msgs/FiducialTransformArray.h>
#include <fiducial_msgs/FiducialTransform.h>
#include <fiducial_msgs/FiducialArray.h>
#include <geometry_msgs/Transform.h>
#include <std_msgs/Header.h>
#include <geometry_msgs/Twist.h>
#include <cob_srvs/SetString.h>
#include <cstdlib>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>

#include "std_srvs/SetBool.h"

namespace ipa_location_finder
{

class LocationListener
{
private:
  ros::Publisher pub;
  ros::Subscriber sub;
  tf::TransformListener listener;
  tf::TransformListener curr_listener;
  ros::ServiceServer service;
  ros::Publisher twist_publisher_;
  ros::Publisher pose_pub_;
  
  bool do_fix_ori;
  bool orientation_fixed;
  bool position_fixed;
  bool goal_reached;
  void Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg);
  
  void orientationAdjust();
  void approachToGoal(double x,double y);
  void listenToTF(double time_now);
  void listenToTF2(double time_now);
  bool check_ori_srv(std_srvs::SetBool::Request &req,
                      std_srvs::SetBool::Response &res );
  
public:
    LocationListener(ros::NodeHandle nh){
    pub = nh.advertise<geometry_msgs::PoseStamped>("Position", 10);
    pose_pub_ = nh.advertise<geometry_msgs::Pose>("marker_pose_2D", 10);
    sub = nh.subscribe("fiducial_transforms",10,&LocationListener::Callback,this); //subscribe to a publish message from fiducial transforms which is published from the aruco_detect
    do_fix_ori = false;
    orientation_fixed = false;
    position_fixed = false;
    goal_reached = false;
    service = nh.advertiseService("orientation_fix", &LocationListener::check_ori_srv,this);
    twist_publisher_ = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    }
   
};





class LocationFinder : public LocationListener
{
private:
//from location finder
    	double time;
		tf::Transform transform_;
		bool marker_detected_;
		bool allow_rotation_;
		bool go;
		ros::Publisher pub;
        ros::Publisher pose_pub_;
		tf::TransformListener listener;
		geometry_msgs::Twist velocity;
		ros::Subscriber sub;
		ros::Subscriber sub2;

        void Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg);
        void Callback_2DOF(const geometry_msgs::PoseStamped::Ptr& msg);


public:
    LocationFinder(ros::NodeHandle nh):LocationListener(nh)
    {
   
    sub = nh.subscribe("fiducial_transforms",10,&LocationFinder::Callback,this); //subscribe to a publish message from fiducial transforms which is published from the aruco_detect
	sub2 = nh.subscribe("Position",10,&LocationFinder::Callback_2DOF,this); // subscribe to a publish message from a Position , that is published in location_listener.cpp
    }
    
};
}


#endif