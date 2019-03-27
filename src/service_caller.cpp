#include <ros/ros.h>
#include <cob_srvs/SetString.h>
#include <cstdlib>
#include <fiducial_msgs/FiducialTransformArray.h>
#include <fiducial_msgs/FiducialTransform.h>
#include <fiducial_msgs/FiducialArray.h>


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


void ServiceCaller::Callservice(){

		cob_srvs::SetString srv;
		srv.request.data = "station_charger";
		if (client.call(srv))
		{
		ROS_INFO("Service is called successfully");
		}
		else
		{
		ROS_ERROR("Failed to call service ");
		
		}
	}

void ServiceCaller::Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg)
{
    if (msg->transforms.empty())
		{
			// ROS_INFO("NO MARKER DETECTED");
		}
		else{
		/// when the marker is detected
//        Callservice();
        }

}
int main(int argc,char** argv){
	ros::init(argc,argv,"location_finder");
	ros::NodeHandle nh;
	
	ServiceCaller ServiceCaller(nh);
	
	ros::spin();
	return 0;
}



