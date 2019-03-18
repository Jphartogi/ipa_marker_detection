#include <location_finder/location_finder.h>

namespace ipa_location_finder
{
 
 void LocationListener::Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg)
 {
   // take current time
   std_msgs::Header header;
	 time = msg->header.stamp.toSec();
   ROS_INFO("time here %f", time);
   listenToTF();
 }

 void LocationListener::listenToTF()
 {

  tf::StampedTransform echo_transform;
  geometry_msgs::PoseStamped ps;
  try
      {
        //listen to a transform from base link to the marker itself
        // Wait for up to one second for the first transforms to become avaiable.
        // listener.waitForTransform("/base_link", "/marker_pose", ros::Time(time), ros::Duration(0));
        
        listener.lookupTransform("/base_link", "/marker_pose", ros::Time(0), echo_transform);
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
   
      pub.publish(ps);

      

 }
}