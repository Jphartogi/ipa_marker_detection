#include <ros/ros.h>
#include <cob_srvs/SetString.h>
#include <cstdlib>
#include <fiducial_msgs/FiducialTransformArray.h>
#include <fiducial_msgs/FiducialTransform.h>
#include <fiducial_msgs/FiducialArray.h>
#include <std_srvs/SetBool.h>

class ServiceCaller
{
private:
	bool docking_srv_called;
	bool publish_now_called;
	int counter;
	double time_delay;
   	ros::Subscriber sub;
   	ros::ServiceClient client;
	ros::ServiceClient pose_client;

	void Dock();
	void publishNow();
   	void Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg);

public:
    ServiceCaller(ros::NodeHandle nh)
    {
	docking_srv_called = false;
	publish_now_called = false;
	counter = 0;
	time_delay = 2;
    sub = nh.subscribe("fiducial_transforms",10,&ServiceCaller::Callback,this); //subscribe to a publish message from fiducial transforms which is published from the aruco_detect
    client =  nh.serviceClient<cob_srvs::SetString>("/docker_control/dock");
	pose_client = nh.serviceClient<std_srvs::SetBool>("/publish_now");

    }
  
};


void ServiceCaller::publishNow(){
		std_srvs::SetBool srv;
		srv.request.data = true;
		if (client.call(srv))
		{
		ROS_INFO("Publish now Service is called successfully");
		publish_now_called = true;
		}
		else
		{
		ROS_ERROR("Failed to call service ");
		
		}



}
void ServiceCaller::Dock(){

		cob_srvs::SetString srv;
		bool goal_status = srv.response.success;
		if (goal_status) {
			// do nothing
		}
		
		if (!docking_srv_called) {
			srv.request.data = "station_charger";
			if (client.call(srv))
			{
			ROS_INFO("Docking Service is called successfully");
			docking_srv_called = true;
			}
			else
			{
			ROS_ERROR("Failed to call service ");
			
			}
		}
		
		
	}

void ServiceCaller::Callback(const fiducial_msgs::FiducialTransformArray::Ptr& msg)
{
	/*
    if (msg->transforms.empty())
		{
			ROS_INFO("NO MARKER DETECTED");
			ros::Duration(5).sleep(); // sleep for 5 second
			// do nothing
		}

	else{
		/// when the marker is detected
		/// we need to publish the TF according to the map orientation 
		if (!publish_now_called) {
			if (counter < 1) { // docking is not yet called just publish the pose
				publishNow();
			}
			else  // docking is called , so it only publish at some rate
			{
				publishNow();
				ros::Duration(time_delay).sleep(); // sleep for x second
			}
		
		}
		
		if (!docking_srv_called && publish_now_called) {
			Dock();
			counter = 1;
		}
		
		

		}
*/

}


int main(int argc,char** argv){
	ros::init(argc,argv,"service_caller");
	ros::NodeHandle nh;
	
	ServiceCaller ServiceCaller(nh);
	
	ros::spin();
	return 0;
}



