#include <location_finder/location_finder.h>

namespace ipa_location_finder
{


void LocationFinder::Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg){
		/// check if the message is empty

		double time = msg->header.stamp.toSec(); // get the time from message
		

		/// check if the message is empty
		if (msg->transforms.empty())
		{
			marker_detected_ = false;
			// this is actually initialization part, if no initialization there will be an error

			std_msgs::Header header;
			time = msg->header.stamp.toSec(); // get the time from message
			// ROS_INFO("No Marker Detected!");
			static tf::TransformBroadcaster br;
			tf::Transform trf2;

			
			if (counter < 1) { // when the marker is not yet detected
				trf2.setOrigin( tf::Vector3(0,0,0));
				trf2.setRotation(tf::Quaternion(0,0,0,1)); //
			}

			else { // when the marker is already detected but goes out of frame

				ros::Time time_after_publish;
				time_after_publish = ros::Time::now();
				double time_after_publish_float = time_after_publish.toSec();
				
				trf2.setOrigin( tf::Vector3(transform_x,transform_y,transform_z));
				trf2.setRotation(tf::Quaternion(rotation_x,rotation_y,rotation_z,rotation_w)); // s
				br.sendTransform(tf::StampedTransform(trf2,ros::Time(time_after_publish_float),"camera_color_optical_frame","marker_pose"));
				header.stamp = time_after_publish;
				time_pub.publish(header);
				ROS_INFO("time publish when no marker %f",time_after_publish_float);

			}

		}

		else{

		/// when the marker is detected
			marker_detected_ = true;

			counter = counter + 1;

			///obtain the pose
			geometry_msgs::Transform transform;
			 transform_x = msg->transforms[0].transform.translation.x;
			 transform_y = msg->transforms[0].transform.translation.y;
			 transform_z = msg->transforms[0].transform.translation.z;
			// ROS_INFO("x: %f,y: %f,z: %f",transform_x,transform_y,transform_z);

			 rotation_x = msg->transforms[0].transform.rotation.x;
			 rotation_y = msg->transforms[0].transform.rotation.y;
			 rotation_z = msg->transforms[0].transform.rotation.z;
			 rotation_w = msg->transforms[0].transform.rotation.w;
			// ROS_INFO("x: %f,y: %f,z: %f,w: %f",rotation_x,rotation_y,rotation_z,rotation_w);
			// obtain the time
			std_msgs::Header header;
		 	ros::Time time_after_publish;

			tf::TransformBroadcaster br;

			transform_base_camera.setOrigin(tf::Vector3(transform_x,transform_y,transform_z));
			transform_base_camera.setRotation(tf::Quaternion(rotation_x,rotation_y,rotation_z,rotation_w));

			// transform_base_camera.setRotation(tf::Quaternion(rotation_x,rotation_y,rotation_z,rotation_w));
			time_after_publish = ros::Time::now();
			double time_after_publish_float = time_after_publish.toSec();

			br.sendTransform(tf::StampedTransform(transform_base_camera,ros::Time(time_after_publish_float),"camera_color_optical_frame","marker_pose"));

			

			ros::Rate rate(1000);

			// publish the time after TF is published
			header.stamp = time_after_publish;
			ROS_INFO("time publish %f",time_after_publish_float);
			time_pub.publish(header);
			// rate.sleep();
			
		}
	}

void LocationFinder::Callback_2DOF(const geometry_msgs::PoseStamped::Ptr& msg){
	// int transform[] = {};
		double waktu = msg->header.stamp.toSec(); // get the time from message
		
		/// check if no marker has detected yet
		if (!marker_detected_)
		{
			static tf::TransformBroadcaster br;
			tf::Transform trf2;
			std_msgs::Header header;
			ros::Time time_after_publish_2DOF;
			time_after_publish_2DOF = ros::Time::now();
			double time_after_publish_float = time_after_publish_2DOF.toSec();

			if (counter < 1) {

				trf2.setOrigin( tf::Vector3(0,0,0));
				trf2.setRotation(tf::Quaternion(0,0,0,1));   // set transform to 0 but not publishing!
			}
			else {

				trf2.setOrigin( tf::Vector3(trans_x,trans_y, 0));
				tf::Quaternion w;
				w.setRPY(0,0,yaw);  // set the 2DOF orientation in Roll Pitch and Yaw. The orientation needed is only the yaw.
				trf2.setRotation(w);
				br.sendTransform(tf::StampedTransform(trf2,ros::Time(time_after_publish_float),"base_link","station_charger_2DOF"));
				ros::Rate rate(1000);
				header.stamp = time_after_publish_2DOF;

				time_pub2.publish(header);
			}

		}

		else{

			///obtain the pose
			projected_2DOF = true;
			std_msgs::Header header;
		 	ros::Time t;

			geometry_msgs::Pose pose;
			trans_x = msg->pose.position.x;
			trans_y = msg->pose.position.y;
			yaw= tf::getYaw(msg->pose.orientation);

			tf::TransformBroadcaster br;
			transform_base_marker.setOrigin( tf::Vector3(trans_x,trans_y, 0));
			tf::Quaternion w;
			w.setRPY(0,0,yaw);  // set the 2DOF orientation in Roll Pitch and Yaw. The orientation needed is only the yaw.
			transform_base_marker.setRotation(w);

			t = ros::Time::now();
			double time_after_publish_float = t.toSec();

			// transform_base_camera.setRotation(tf::Quaternion(rotation_x,rotation_y,rotation_z,rotation_w));
      		br.sendTransform(tf::StampedTransform(transform_base_marker,ros::Time(time_after_publish_float),"base_link","station_charger_2DOF"));
			ros::Rate rate(1000);


			
			// publish the time after TF is published
			header.stamp = t;

			time_pub2.publish(header);


			// rate.sleep();

	}

}
}

