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

namespace ipa_location_finder
{

class LocationListener
{
private:
  double time;
  ros::Publisher pub;
  ros::Subscriber sub;
  tf::TransformListener listener;
  void Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg);
  void listenToTF();
  
public:
    LocationListener(ros::NodeHandle nh){
    pub = nh.advertise<geometry_msgs::PoseStamped>("Position", 10);
   sub = nh.subscribe("fiducial_transforms",10,&LocationListener::Callback,this); //subscribe to a publish message from fiducial transforms which is published from the aruco_detect
    }
   
};



class ServiceCaller
{
private:
   ros::Subscriber sub;
   void Callservice();
   void Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg);
   ros::ServiceClient client;
public:
    ServiceCaller(ros::NodeHandle nh)
    {
    sub = nh.subscribe("fiducial_transforms",10,&ServiceCaller::Callback,this); //subscribe to a publish message from fiducial transforms which is published from the aruco_detect
    client =  nh.serviceClient<cob_srvs::SetString>("/docker_control/dock");
    }
  
};



class LocationFinder : public LocationListener, public ServiceCaller
{
private:
//from location finder
    	double time;
		tf::Transform transform_;
		bool marker_detected_;
		bool allow_rotation_;
		bool go;
		ros::Publisher pub;
		tf::TransformListener listener;
		geometry_msgs::Twist velocity;
		ros::Subscriber sub;
		ros::Subscriber sub2;

        void Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg);
        void Callback_2DOF(const geometry_msgs::PoseStamped::Ptr& msg);


public:
    LocationFinder(ros::NodeHandle nh):LocationListener(nh), ServiceCaller(nh)
    {
    sub = nh.subscribe("fiducial_transforms",10,&LocationFinder::Callback,this); //subscribe to a publish message from fiducial transforms which is published from the aruco_detect
	sub2 = nh.subscribe("Position",10,&LocationFinder::Callback_2DOF,this); // subscribe to a publish message from a Position , that is published in location_listener.cpp
    }
    
};
}


#endif