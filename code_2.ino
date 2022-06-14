//Receiver Code
#include<SoftwareSerial.h>

#define DEBUG true

#define SoftwareSerial_RX 4
#define SoftwareSerial_TX 2

//IB pins are connected to PWM Pins on Arduino
#define MD1_IA 7
#define MD1_IB 5
#define MD2_IA 8
#define MD2_IB 6
#define MD3_IA 13
#define MD3_IB 10
#define MD4_IA 12
#define MD4_IB 11

//Motor Sets
#define Motor1 1  //Left 
#define Motor2 2  //Right
#define Motor3 3  //Left
#define Motor4 4  //Right

SoftwareSerial HC12_Serial(SoftwareSerial_RX,SoftwareSerial_TX); 

struct JoyStickData {
  uint32_t JS1_X;
  uint32_t JS1_Y;

  uint32_t JS2_X;
  uint32_t JS2_Y;
};

JoyStickData MyData;
void setup() {
  // put your setup code here, to run once:
  HC12_Serial.begin(9600);
  Serial.begin(9600);
  
  //Initializatoin
  pinMode(MD1_IA,OUTPUT);
  pinMode(MD1_IB,OUTPUT);
  pinMode(MD2_IA,OUTPUT);
  pinMode(MD2_IB,OUTPUT);
  pinMode(MD3_IA,OUTPUT);
  pinMode(MD3_IB,OUTPUT);
  pinMode(MD4_IA,OUTPUT);
  pinMode(MD4_IB,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(HC12_Serial.available()){
    HC12_Serial.readBytes((byte*)&MyData, sizeof(MyData));

    //Serial Data Out for Debug
    if(DEBUG){
      Serial.print("JS1_X:");
      Serial.print(MyData.JS1_X);
      Serial.print("JS1_Y:");
      Serial.print(MyData.JS1_Y);
      Serial.print("JS2_X:");
      Serial.print(MyData.JS2_X);
      Serial.print("JS2_Y:");
      Serial.println(MyData.JS2_Y);
    }

    MotorDrive(Motor1, MyData.JS1_Y);
    MotorDrive(Motor3, MyData.JS1_Y);
    MotorDrive(Motor2, MyData.JS2_Y);
    MotorDrive(Motor4, MyData.JS2_Y);
  }
}


void MotorRun(uint8_t Pin_A, uint8_t Pin_B, int speed){
  if(speed<0){
    digitalWrite(Pin_A, 1);
    analogWrite(Pin_B, 255+speed);
  }else{
    digitalWrite(Pin_A, 0);
    analogWrite(Pin_B, speed);
  }
}

void MotorDrive(uint8_t motor, int speed){
  switch(motor){
    case Motor1 : {
        MotorRun(MD1_IA,MD1_IB,map(speed,0,1024,-255,255));
      break;
    }
    case Motor2 : {
      MotorRun(MD2_IA,MD2_IB,map(speed,0,1024,-255,255));
      break;
    }
    case Motor3 : {
      MotorRun(MD3_IA,MD3_IB,map(speed,0,1024,-255,255));
      break;
    }
    case Motor4 : {
      MotorRun(MD4_IA,MD4_IB,map(speed,0,1024,-255,255));
      break;
    }

  }

}
