#include <ros/ros.h>
#include <std_msgs/Int16.h>


void chatterCallback(const std_msgs::Int16::ConstPtr& msg){
  if(msg->data%3==0&&msg->data%5==0){
    ROS_INFO("FizzBuzz");
  }else if(msg->data%3==0){
    ROS_INFO("Fizz");
  }else if(msg->data%5==0){
    ROS_INFO("Buzz");
  }else{
    ROS_INFO("%d",msg->data);
  }
}

int main(int argc,char *argv[]){
	ros::init(argc,argv,"listener");
	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe
		("chatter",1000,chatterCallback);

	ros::spin();
	return 0;
}
