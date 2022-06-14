//Transmitter Code
#include<SoftwareSerial.h>

#define DEBUG true

#define SoftwareSerial_RX 4
#define SoftwareSerial_TX 2

#define JS1_X_Pin A1
#define JS1_Y_Pin A0

#define JS2_X_Pin A3
#define JS2_Y_Pin A2

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

  //initallization
  pinMode(JS1_X_Pin, INPUT);
  pinMode(JS1_Y_Pin, INPUT);
  pinMode(JS2_X_Pin, INPUT);
  pinMode(JS2_Y_Pin, INPUT);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  MyData.JS1_X = analogRead(JS1_X_Pin);
  MyData.JS1_Y = analogRead(JS1_Y_Pin);
  MyData.JS2_X = analogRead(JS2_X_Pin);
  MyData.JS2_Y = analogRead(JS2_Y_Pin);

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
  HC12_Serial.write((byte*)&MyData, sizeof(MyData));

  delay(100);
}
