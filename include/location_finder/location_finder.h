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
  ros::Subscriber sub2;
  tf::TransformListener listener;
  tf::TransformListener listener_map;
  ros::ServiceServer service;
  ros::Publisher twist_publisher_;
  ros::Publisher pose_pub_;

  void Callback(const std_msgs::Header::Ptr& msg);
  void Callback2(const std_msgs::Header::Ptr& msg);

  void listenToTF(double time_now);

  void listenToTF2(double time_now2);
  
  

public:
    LocationListener(ros::NodeHandle nh){
    pub = nh.advertise<geometry_msgs::PoseStamped>("marker_pose_2D", 100);
    pose_pub_ = nh.advertise<geometry_msgs::PoseStamped>("marker_pose_map", 100);
    sub = nh.subscribe("time_after_pub_2DOF",10,&LocationListener::Callback,this);
    sub2 = nh.subscribe("time_after_pub_2DOF_real",10,&LocationListener::Callback2,this);
    }

};

class LocationFinder
{
private:
//from location finder
  double transform_x , transform_y , transform_z;
  double rotation_x , rotation_y , rotation_z , rotation_w;
  
  double trans_x , trans_y , yaw;

  double trf_x , trf_y , ori_yaw;

    int counter;
		
		bool marker_detected_;
		
    bool projected_2DOF;
		ros::Publisher time_pub;
    ros::Publisher time_pub2;
		tf::Transform transform_base_camera;
    tf::Transform transform_base_marker;
		
		ros::Subscriber sub;
		ros::Subscriber sub2;
    ros::Subscriber sub3;

    void Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg);
    void Callback_2DOF(const geometry_msgs::PoseStamped::Ptr& msg);
    

public:
    LocationFinder(ros::NodeHandle nh)
    {
      // initialize the TF
      transform_x = 0; transform_y = 0; transform_z = 0;
      rotation_x = 0; rotation_y = 0; rotation_z = 0; rotation_w = 0;

      trans_x = 0; trans_y = 0; yaw = 0;

      marker_detected_ = false;
      projected_2DOF = false;
      counter = 0;
      time_pub = nh.advertise<std_msgs::Header>("time_after_pub_2DOF",10);
      time_pub2 = nh.advertise<std_msgs::Header>("time_after_pub_2DOF_real",10);
      sub = nh.subscribe("fiducial_transforms",10,&LocationFinder::Callback,this); //subscribe to a publish message from fiducial transforms which is published from the aruco_detect
	    sub2 = nh.subscribe("marker_pose_2D",10,&LocationFinder::Callback_2DOF,this); // subscribe to a publish message from a Position , that is published in location_listener.cpp
      // sub3 = nh.subscribe("marker_pose_map",10,&LocationFinder::Callback_Map,this);
    }

};
}


#endif
