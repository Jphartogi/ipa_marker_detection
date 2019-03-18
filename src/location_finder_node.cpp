#include <location_finder/location_finder.h>
#include <ros/ros.h>

int main(int argc,char** argv){
	ros::init(argc,argv,"location_finder");
	ros::NodeHandle nh;
	ipa_location_finder::LocationFinder LocationFinder(nh);
	// ipa_location_finder::LocationListener LocationListener(nh);
	// ipa_location_finder::ServiceCaller ServiceCaller(nh);
	
	ros::spin();
	return 0;
}

