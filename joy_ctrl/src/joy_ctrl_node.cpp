#include <ros/ros.h>
#include <std_msgs/Int8.h>
#include <sensor_msgs/Joy.h>

#define AXIS_LEFT_Y 1

int ctrl = 0;

void joyCallback(const sensor_msgs::Joy::ConstPtr& msg){
	if(msg->axes[AXIS_LEFT_Y] > 0.2)ctrl = 1;
	else if(msg->axes[AXIS_LEFT_Y] < -0.2)ctrl = -1;
	else ctrl = 0;
}

int main(int argc,char *argv[]){
	ros::init(argc,argv,"joy_ctrl_node");
	ros::NodeHandle n;
	ros::Rate loop_rate(100);
		
	ros::Publisher pub = n.advertise<std_msgs::Int8>
		("control",1000);
	ros::Subscriber sub = n.subscribe<sensor_msgs::Joy>
		("joy",1000,joyCallback);
	while(ros::ok()){
		std_msgs::Int8 msg;
		msg.data = ctrl;
		pub.publish(msg);
	
		loop_rate.sleep();
		ros::spinOnce();
	}
	return 0;
}

