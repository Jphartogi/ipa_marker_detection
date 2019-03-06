#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>

#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>

class send_location
{
private:
    /* data */
public:
    send_location(/* args */);
    ~send_location();
    void callback(const geometry_msgs::PoseStamped::Ptr& pose);
};


void send_location::callback(const geometry_msgs::PoseStamped::Ptr& msg){


            geometry_msgs::PoseStamped ps;
            ///obtain the pose
			geometry_msgs::Pose pose;
			double transform_x = msg->pose.position.x;
			double transform_y = msg->pose.position.y;

			
            ps.pose.position.x = msg->pose.position.x;
            ps.pose.position.y = msg->pose.position.y;
            ps.pose.position.z = msg->pose.position.z;

            ps.pose.orientation = msg->pose.orientation;

    //start a action client to send the pose to the Server


			actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("/docker_control/move_base_linear", true);

			ROS_INFO("Waiting for action server to start");
			ac.waitForServer();
		
			ROS_INFO("Action server started, sending goal ");
			move_base_msgs::MoveBaseGoal goal;
			geometry_msgs::PoseStamped target_pose;

			// goal.target_pose.pose.position.x = transform_x;
			// goal.target_pose.pose.position.y = transform_y;
			// goal.target_pose.pose.position.z = 0;

			// goal.target_pose.pose.orientation = msg->pose.orientation;
            goal.target_pose = ps;
            goal.target_pose.header.frame_id = "station_charger";
      

			ac.sendGoal(goal);

			bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));
			 if (finished_before_timeout)
  			{
				actionlib::SimpleClientGoalState state = ac.getState();
				ROS_INFO("Action finished: %s",state.toString().c_str());
			}
			else
				ROS_INFO("Action did not finish before the time out.");

			
			}





send_location::send_location(/* args */)
{
}

send_location::~send_location()
{
}



int main(int argc,char *argv[]){
	ros::init(argc,argv,"location_finder");
	ros::NodeHandle nh;
	send_location location;

	ros::Subscriber sub = nh.subscribe("Position",10,&send_location::callback,&location); // subscribe to a publish message from a Position , that is published in location_listener.cpp

  ros::spin();
	return 0;
}
