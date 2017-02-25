//--ROS 通信用--
#include <ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int16MultiArray.h>

//--バッテリ--
#include <MsTimer2.h>

//--モータ--

ros::NodeHandle nh;

ros::Publisher chatterBATTERY("sensors",&battery_value);
ros::Subscriber<std_msgs::Int16MultiArray> subscriberPWM("send_arduino",&cmdPWMCallBack);

void setup(){
  
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  
  nh.advertise(chatterBATTERY);
  nh.subscribe(subscriberPWM);
  
  MsTimer2::set(1000,read_voltage);//read_voltage is not written yet.
  MsTimer2::start();
}

void loop(){
  
}
