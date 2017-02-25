#define USB_USBCON

#include <ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int16MultiArray.h>


// ピン番号の定義
// モータ1（右）
// PWM: Pin 3, dir: Pin 2, encoder(A&B): Pin 4, 5
const int PIN_MOTOR_PWM_1 = 3;
const int PIN_MOTOR_DIR_1 = 2;
const int PIN_ENCODER_1A = 4;
const int PIN_ENCODER_1B = 5;
// モータ2（左）
// PWM: Pin 11, dir: Pin 12, encoder(A&B): Pin 6, 7
const int PIN_MOTOR_PWM_2 = 11;
const int PIN_MOTOR_DIR_2 = 12;
const int PIN_ENCODER_2A = 6;
const int PIN_ENCODER_2B = 7;

// パラメータ
const int GEAR_RATIO = 64;  // ギア比
const int RES_ENC = 12;     // エンコーダ分解能（1回転あたりの出力パルス数）

ros::NodeHandle nh;

std_msgs::Int16 pwmRight,pwmLeft;

void moveMotor();

void cmdPWMCallBack(const std_msgs::Int16MultiArray& msg){
  pwmRight.data = msg.data[0];
  pwmLeft.data = msg.data[1];

  moveMotor();
}

void moveMotor(){

  if(pwmRight.data >= 0){
    digitalWrite(PIN_MOTOR_DIR_1,HIGH);
    analogWrite(PIN_MOTOR_PWM_1,abs(pwmRight.data));
  }else if(pwmRight.data < 0){
    digitalWrite(PIN_MOTOR_DIR_1,LOW);
    analogWrite(PIN_MOTOR_PWM_2,abs(pwmRight.data));
  }
  
  if(pwmLeft.data >= 0){
    digitalWrite(PIN_MOTOR_DIR_2,HIGH);
    analogWrite(PIN_MOTOR_PWM_2,abs(pwmLeft.data));
  }else if(pwmLeft.data < 0){
    digitalWrite(PIN_MOTOR_DIR_2,LOW);
    analogWrite(PIN_MOTOR_PWM_2,abs(pwmLeft.data));
  }
  
  
}

ros::Subscriber <std_msgs::Int16MultiArray> subscriberPWM("send_arduino",&cmdPWMCallBack);


void setup(){
  
  nh.getHardware() -> setBaud(57600);
  nh.initNode();
  nh.subscribe(subscriberPWM);

  
  // change PWM freq
  TCCR1B=TCCR1B&0xf8|0x01;  // Pin9,Pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;  // Pin3,Pin11 PWM 31250Hz

  // ピンモードの設定
  pinMode(PIN_MOTOR_PWM_1, OUTPUT);
  pinMode(PIN_MOTOR_PWM_2, OUTPUT);
  pinMode(PIN_ENCODER_1A, INPUT);
  pinMode(PIN_ENCODER_1B, INPUT);
  pinMode(PIN_ENCODER_2A, INPUT);
  pinMode(PIN_ENCODER_2B, INPUT);


}

void loop(){
  nh.spinOnce();
  delay(1);
}
