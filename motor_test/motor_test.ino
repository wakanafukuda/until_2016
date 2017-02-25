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

int voltage1,voltage2;

void riseEnc1A(){
  
  
}

void riseEnc2A(){
  
  
}

void moveMotor1(){
  digitalWrite(PIN_MOTOR_DIR_1,HIGH);
  analogWrite(PIN_MOTOR_PWM_1,voltage1); 
}

void moveMotor2(){
  digitalWrite(PIN_MOTOR_DIR_2,HIGH);
  analogWrite(PIN_MOTOR_PWM_2,voltage2);
}

void setup(){
  
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
  
  
    digitalWrite(PIN_MOTOR_DIR_1,HIGH);
    analogWrite(PIN_MOTOR_PWM_1,100);
/*
    digitalWrite(PIN_MOTOR_DIR_2,HIGH);
    analogWrite(PIN_MOTOR_PWM_2,100);
    delay(5000);
    
    digitalWrite(PIN_MOTOR_DIR_1,HIGH);
    analogWrite(PIN_MOTOR_PWM_1,0);
    digitalWrite(PIN_MOTOR_DIR_2,HIGH);
    analogWrite(PIN_MOTOR_PWM_2,0);
    delay(5000);
*/    
 
}
