#include <location_finder/location_finder.h>



namespace ipa_location_finder
{

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
        Callservice();
        }

}
}


