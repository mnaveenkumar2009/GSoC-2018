#include <Wire.h>

int led = 13;
int errorLed = 12;
int led1 = 11;
int led2 = 10;
int button1 = 6;
int button2 = 7;

unsigned char SLA = 4;	// slave address
unsigned char sendData = 0;
unsigned char getData = 0;

// the setup routine runs once when you press reset:
void setup() {
  pinMode(led, OUTPUT);
  pinMode(errorLed, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  Wire.begin();//since it is master, address is optional
  Serial.begin(9600);//start serial o/p
  
}

int old_time=millis();
int new_time=old_time;
bool state =0;
void loop() {
  new_time=millis();
  //Serial.println(old_time);
  //Serial.println(new_time);
  //Serial.println("b");
  if(new_time-old_time>=1000){
    old_time=new_time;
    state=!state;    
    digitalWrite(led, state);
    Serial.println(new_time);
  }
  
  Wire.requestFrom(SLA, 1);    // request 1 byte from slave device

  while (Wire.available()) { // slave may send less than requested
    int x = Wire.read(); // receive a byte 
    digitalWrite(led1, x & 1);
    digitalWrite(led2, (x >> 1) & 1);
  }
  
  sendData=0;
  if (!digitalRead(button1)) sendData |= 1;
  if (!digitalRead(button2)) sendData |= 1<<1;

  Wire.beginTransmission(SLA);
  Wire.write(sendData);
  Wire.endTransmission();
}