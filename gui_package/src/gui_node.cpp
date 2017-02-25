#include<ros/ros.h>
#include<stdlib.h>

int main(int argc,char *argv[]){
  ros::init(argc,argv,"gui_node");
  ros::NodeHandle n;

  system("firefox /home/wakana/rosjstest/gui.html");
  return 0;
}
