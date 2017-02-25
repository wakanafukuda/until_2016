#include <ros.h>
#include <std_msgs/Int16.h>

ros::NodeHandle nh;

std_msgs::Int16 cmdpwm;
ros::Publisher chatter("chatter",&cmdpwm);

void cmdPWMCallBack(const std_msgs::Int16& msg){
  cmdpwm = msg;
  chatter.publish(&cmdpwm);
}

ros::Subscriber<std_msgs::Int16> sub("movement",&cmdPWMCallBack);

void setup(){
  nh.initNode();
  nh.advertise(chatter);
  nh.subscribe(sub);
}

void loop(){
  nh.spinOnce();
  delay(1000);
}
