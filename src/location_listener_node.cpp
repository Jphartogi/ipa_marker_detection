#include <location_finder/location_finder.h>
#include <ros/ros.h>

int main(int argc,char** argv){
	ros::init(argc,argv,"location_listener_node");
	ros::NodeHandle nh;
	ipa_location_finder::LocationListener LocationListener(nh);

	
	ros::spin();
	return 0;
}

