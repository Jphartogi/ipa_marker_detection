#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Pose.h>
#include <std_srvs/SetBool.h>
#include <std_msgs/Header.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <cstdlib>

class LocationPublisher
{
private:
    ros::Subscriber sub;
    ros::ServiceServer service;
    bool publish_now;
    int counter;

    double transform_x;
    double transform_y;
    double yaw;

    bool publish_pose(std_srvs::SetBool::Request &req,
                      std_srvs::SetBool::Response &res );
    void Callback(const geometry_msgs::PoseStamped::Ptr& msg);
public:
    LocationPublisher(ros::NodeHandle nh);


};


LocationPublisher::LocationPublisher(ros::NodeHandle nh)
{
    sub = nh.subscribe("marker_pose_map",10,&LocationPublisher::Callback,this);
    service = nh.advertiseService("publish_now", &LocationPublisher::publish_pose,this);
    publish_now = false;
    counter = 0;
}

bool LocationPublisher::publish_pose(std_srvs::SetBool::Request &req,
                      std_srvs::SetBool::Response &res )
		{
		if (req.data == true) {
            publish_now = true;
            res.success = true;
            res.message = "service is called successfully";
	          counter = 0; // reset the counter so taking a new value of the marker pose
            return true;
			}
		else
			{
			res.success = false;
			res.message = "service is failed to be called ";
			publish_now = false;
            return false;
			}
		}



void LocationPublisher::Callback(const geometry_msgs::PoseStamped::Ptr& msg)
{
    if (publish_now == true) { // when the service is called

        // for taking the data just once
		if ( counter < 1)
		{

            /// when the marker is detected

			///obtain the pose
			geometry_msgs::Pose pose;

			transform_x = msg->pose.position.x;
			transform_y = msg->pose.position.y;
			yaw= tf::getYaw(msg->pose.orientation);
			counter = counter + 1;  // used for only taking data once and but keep publishing at constant rate!

		}
		else{
			static tf::TransformBroadcaster br;
			tf::Transform transform_marker_map;
			transform_marker_map.setOrigin( tf::Vector3(transform_x,transform_y, 0));
			tf::Quaternion w;
			w.setRPY(0,0,yaw);  // set the 2DOF orientation in Roll Pitch and Yaw. The orientation needed is only the yaw.
			transform_marker_map.setRotation(w);
		// transform_base_camera.setRotation(tf::Quaternion(rotation_x,rotation_y,rotation_z,rotation_w));
            br.sendTransform(tf::StampedTransform(transform_marker_map,ros::Time::now(),"map","station_charger_2DOF"));
		}
    }
    else
    {
        ROS_INFO("waiting for service!");
    }


}

int main(int argc, char** argv)
{
    ros::init(argc,argv,"location_publisher");
    ros::NodeHandle nh;

    LocationPublisher LocationPublisher(nh);

    ros::spin();
    return 0;
}
