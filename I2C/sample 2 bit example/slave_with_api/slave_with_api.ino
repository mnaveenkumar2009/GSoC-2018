#include <Wire.h>

int led = 13;
int errorLed = 4;
int led1 = 6;
int led2 = 5;
int button1 = 7;
int button2 = 8;

unsigned char SLA = 4;		// slave address
unsigned char sendData = 0;
unsigned char getData = 0;

// the setup routine runs once when you press reset:
void setup() {
  pinMode(led, OUTPUT);
  pinMode(errorLed, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(button1, INPUT_PULLUP);//bit1
  pinMode(button2, INPUT_PULLUP);//bit2
  Serial.begin(9600);
  Wire.begin(SLA);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}

void receiveEvent(int bytes){
  int x = Wire.read();    // receive byte as an integer
  //Serial.println(x); 
  digitalWrite(led1, x & 1);
  digitalWrite(led2, (x >> 1) & 1);
  //Wire.endTransmission();
}

void requestEvent() {
  sendData = 0;
  if (!digitalRead(button1)) sendData |= 1;
  if (!digitalRead(button2)) sendData |= 1<<1;
  //Serial.println("kjb");
  Wire.write(sendData);
}