#include <location_finder/location_finder.h>

namespace ipa_location_finder
{

 void LocationListener::Callback(const std_msgs::Header::Ptr& msg)
 {
   // take the time when the TF is already published

   double time_now = msg->stamp.toSec(); 


   listenToTF(time_now); // send the time for the listener so it listen only when the TF is already published

 }

 void LocationListener::Callback2(const std_msgs::Header::Ptr& msg)
 {
   // take the time when the TF is already published

   double time_now2 = msg->stamp.toSec();


   listenToTF2(time_now2); // send the time for the listener so it listen only when the TF is already published


 }

 void LocationListener::listenToTF(double time_now)
 {
  ROS_INFO("time before listen base marker %f",time_now);
  tf::StampedTransform echo_transform;
  geometry_msgs::PoseStamped ps;

      try
      {
        //listen to a transform from base link to the marker itself
        // listener.waitForTransform("/base_link", "/marker_pose", ros::Time(time_now), ros::Duration(0.5));
        listener.lookupTransform("/base_link", "/marker_pose", ros::Time(time_now), echo_transform);
       }
       catch(tf::TransformException& ex)
      {
         ROS_ERROR("%s",ex.what());
         ros::Duration(0.1).sleep();

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

      ps.header.stamp = ros::Time::now(); // send time after success listen to TF

      double time_after_listen = ros::Time::now().toSec();
      // ROS_INFO("time after listen base marker %f",time_after_listen);
      pub.publish(ps);


 }

   void LocationListener::listenToTF2(double time_now)
 {

  tf::StampedTransform marker_base_transform;
  geometry_msgs::PoseStamped ps;
    try
      {
        //listen to a transform from base link to the marker itself
        // listener.waitForTransform("/base_link", "/marker_pose", ros::Time(time), ros::Duration(0));
        listener.lookupTransform("/map", "/station_charger_2DOF", ros::Time(time_now), marker_base_transform);
       }
       catch(tf::TransformException& ex)
      {
         ROS_ERROR("%s",ex.what());
         ros::Duration(0.1).sleep();
         
      }
      // double yaw, pitch, roll;
      // echo_transform.getBasis().getRPY(roll, pitch, yaw);
      tf::Quaternion q = marker_base_transform.getRotation();
      tf::Vector3 v = marker_base_transform.getOrigin();
      
      // fiducial_msgs::FiducialTransform ft;
      ps.pose.position.x = v.getX();
      ps.pose.position.y = v.getY();
      ps.pose.position.z = 0;

      ps.pose.orientation.w = q.getW(); //take value of yaw to be inserted one of quaternion
      ps.pose.orientation.x = q.getX();
      ps.pose.orientation.y = q.getY();
      ps.pose.orientation.z = q.getZ();


      ps.header.stamp = ros::Time::now();  // send time after success listen to TF

      pose_pub_.publish(ps);
    

 }
}


 
