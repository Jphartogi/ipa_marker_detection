#include <location_finder/location_finder.h>

namespace ipa_location_finder
{
 bool LocationListener::check_ori_srv(std_srvs::SetBool::Request &req,
                      std_srvs::SetBool::Response &res )
		{
			if (req.data == true) {
            do_fix_ori = true;
            res.success = true;
            res.message = "service is called successfully";
            return true;
			}
			else
			{
				res.success = false;
				res.message = "service is failed to be called ";
				do_fix_ori = false;
			}
		}
 void LocationListener::Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg)
 {
   // take current time
   std_msgs::Header header;
	 time = msg->header.stamp.toSec();
  //  ROS_INFO("time here %f", time);
   listenToTF();
   orientationAdjust();
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

 void LocationListener::orientationAdjust()
  {
    if (do_fix_ori) {
      
      tf::StampedTransform transform;
      geometry_msgs::PoseStamped ps;
    try
      {
        listener.lookupTransform("/station_charger", "/base_link", ros::Time::now(), transform);
       }
       catch(tf::TransformException& ex)
      {
         ROS_ERROR("%s",ex.what());
         ros::Duration(0.1).sleep();
      }
      // double yaw, pitch, roll;
      // transform.getBasis().getRPY(roll, pitch, yaw);
      tf::Quaternion q = transform.getRotation();
      tf::Vector3 v = transform.getOrigin();
      geometry_msgs::Pose pose;
      // fiducial_msgs::FiducialTransform ft;
      ps.pose.position.x = v.getX();
      ps.pose.position.y = v.getY();
      ps.pose.position.z = 0;

      ps.pose.orientation.w = q.getW(); //take value of yaw to be inserted one of quaternion
      ps.pose.orientation.x = q.getX();
      ps.pose.orientation.y = q.getY();
      ps.pose.orientation.z = q.getZ();

      double yaw= tf::getYaw(ps.pose.orientation);
      approachToGoal(ps.pose.position.x,ps.pose.position.y);
      std::cout << "what is the yaw " << yaw << std::endl;
      geometry_msgs::Twist twist;

      if (!orientation_fixed) {
          if (0.05 > yaw && yaw > -0.05) {
          twist.angular.z = 0.00;
          ROS_INFO("orientation is now the same");
          twist_publisher_.publish(twist);
          orientation_fixed = true;
          }
          else
          {
          ROS_INFO("kesini ga sih dia");
           if (yaw > 0 ) {
              if (yaw < 0.6) {
                twist.angular.z = -0.04;
                twist_publisher_.publish(twist);
                
              }
              else
              {
                twist.angular.z = -0.2;
                twist_publisher_.publish(twist);
              }
            }
            else 
            {
              if (yaw > -0.6) {
                twist.angular.z = 0.04;
                twist_publisher_.publish(twist);
              }
              else
              {
                twist.angular.z = 0.2;
                twist_publisher_.publish(twist);
              }
                
            }
          }
      }
      
     
    }
    
  }

  void LocationListener::approachToGoal(double x,double y)
  {
    geometry_msgs::Twist twist;
    double tolerance = 0.1;
    double y_tolerance = 0.02;
    if (!goal_reached) {
        if (y < y_tolerance && y > -y_tolerance) {
          position_fixed = true;
          if (orientation_fixed && position_fixed)
          {
            if (x < tolerance && x > -tolerance) {
              twist.linear.x = 0.001;
              twist_publisher_.publish(twist);
              ROS_INFO("dia berhenti");
              goal_reached = true;
              
            }
            else
            {
              twist.linear.x = 0.05;
              twist_publisher_.publish(twist);
              ROS_INFO("dia mendekat");
            }

          }

        }
        else
        {
          /* code */
        }
    }
    
      
      
    
    
    
    
    
    
  }
}