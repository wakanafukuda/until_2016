#include <ros/ros.h>
#include <std_msgs/Int16.h>

int main(int argc,char *argv[]){

		ros::init(argc,argv,"talker");
		ros::NodeHandle n;
	
		ros::Rate loop_rate(10);
	
		ros::Publisher
			pub = n.advertise<std_msgs::Int16>("chatter",1000);
		int count = 1;
		while(ros::ok()){
		std_msgs::Int16 msg;
		msg.data = count;
		pub.publish(msg);
		loop_rate.sleep();
		count++;
		}
	return 0;
}
