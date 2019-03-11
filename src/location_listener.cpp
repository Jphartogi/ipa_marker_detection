#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>

//#include <location_finder/Position_publish.h>
#include "fiducial_msgs/FiducialTransform.h"
#include "fiducial_msgs/FiducialTransformArray.h"

 class Locationlistener
 {
 public:

   tf::TransformListener listener;

 private:

 };

 int main(int argc, char ** argv)
 {
   //Initialize ROS
   ros::init(argc, argv, "location_finder");
   ros::NodeHandle nh;
   // ros::Publisher pub = nh.advertise<fiducial_msgs::FiducialTransformArray>("Position", 10);
   ros::Publisher pub = nh.advertise<geometry_msgs::PoseStamped>("Position", 10);
   //Instantiate a local listener
   Locationlistener Locationlistener;

   ros::Rate rate(1000);
   while(nh.ok())
     {
      tf::StampedTransform echo_transform;
      geometry_msgs::PoseStamped ps;
       try
       {
        //listen to a transform from base link to the marker itself
        // Wait for up to one second for the first transforms to become avaiable.
        // Locationlistener.listener.waitForTransform("/base_link", "/marker_pose", ros::Time(0), ros::Duration(1.0));
        Locationlistener.listener.lookupTransform("/base_link", "/station_charger", ros::Time(0), echo_transform);
       }
       catch(tf::TransformException& ex)
       {
         ROS_ERROR("%s",ex.what());
         ros::Duration(0.1).sleep();
         continue;
       }
      // double yaw, pitch, roll;
      // echo_transform.getBasis().getRPY(roll, pitch, yaw);
      tf::Quaternion q = echo_transform.getRotation();
      tf::Vector3 v = echo_transform.getOrigin();
      geometry_msgs::Pose pose;
      // fiducial_msgs::FiducialTransform ft;
      ps.pose.position.x = v.getX();
      ps.pose.position.y = v.getY();
      ps.pose.position.z = 0;

      ps.pose.orientation.w = q.getW(); //take value of yaw to be inserted one of quaternion
      ps.pose.orientation.x = q.getX();
      ps.pose.orientation.y = q.getY();
      ps.pose.orientation.z = q.getZ();

      // double goal_rot = tf::getYaw(ps.pose.orientation);


    //       // publish the transform
      pub.publish(ps);


        
     }    

     
   return 0;
 }
 
