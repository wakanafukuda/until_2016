#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <cstdlib> //for atoi()
/*
void movementCallBack(const std_msgs::Int16& msg){
  ROS_INFO("I heared : [ %d ]",msg->data);
}
*/

std::string robot_mode;
void movementCallBack(const msg2015::CmdPWM& msg){
  int r_mode;
  int r_motor[6];
  int i;

  if(n.getParam("/robot_mode",robot_mode)){
      r_mode = atoi(robot_mode);
      if(r_mode == 0){
	for(i = 0;i < 6;i++){
	  r_motor[i] = 0;
	}
      }else if(r_mode == 1){
	//	calcMotor();
      }
    }
    ROS_INFO("Robot Mode: %d",r_mode);
    ino_pub.pub(r_motor);
}

//calcMotor();

int main(int argc, char** argv){
  ros::init(argc,argv,"operate_robot");

  ros::NodeHandle n;

  ros::Publisher ino_pub = n.advertise<std_msgs::Int16>("send_arduino",1000);
  ros::Subscriber sub_state = n.subscribe("movement",1000,movementCallBack);
