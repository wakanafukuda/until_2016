#include <ros/ros.h>


int main(int argc,char *argv[]){

	ros::init(argc,argv,"beginner");
	ros::NodeHandle n;

	ros::Rate loop_rate(10);

	while(ros::ok()){
		ROS_INFO("Hello World!");
		loop_rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
