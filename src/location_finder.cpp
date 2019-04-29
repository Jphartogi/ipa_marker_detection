#include <location_finder/location_finder.h>

namespace ipa_location_finder
{


void LocationFinder::Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg){
		/// check if the message is empty
		if (msg->transforms.empty())
		{
			// this is actually initialization part, if no initialization there will be an error

			std_msgs::Header header;
			time = msg->header.stamp.toSec(); // get the time from message
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

void LocationFinder::Callback_2DOF(const geometry_msgs::PoseStamped::Ptr& msg){
	// int transform[] = {};

		/// check if no marker has detected yet
		if (!marker_detected_)
		{
			
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
            br.sendTransform(tf::StampedTransform(transform_base_marker,ros::Time(time),"base_link","station_charger_real"));
			ros::Rate rate(1000);


			// rate.sleep();
		
	}

}


	
}
