#include<ros.h>
#include<std_msgs/Int16.h>

ros::NodeHandle nh;


ros::Publisher<std_msgs::Int16> 
  pub = n.("control", 1000);

void setup(){
     nh.initNode();
     nh.publish(pub);
     pinMode(A0,INPUT);
}

void loop(){
   nh.spinOnce();
   analogRead(A0);
   delay(10);
}
