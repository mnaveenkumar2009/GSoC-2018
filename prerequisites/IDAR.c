/*
FILE : IDAR.c (Interrupt Driven Analog Read)
Objective :
    The following program illustrates a simple program that check's the user's knowledge of prime numbers using interrupt driven analog reads.
    The objective of the user is to decide whether the number displayed on the LCD is prime or not.
    The user needs to push the button connected to pin 2 if he thinks the number is a prime number.
    The user needs to push the button connected to pin 3 if he thinks the number is a composite number.
    The user's score and the correctness of the answer are displayed.
    Each turn a new random number is generated.
    
Circuit : https://www.tinkercad.com/things/91BmknNjFda
        : IDAR.png

Circuit Details :
    Components used:
        1 Arduino (DFRduino R3 used in circuit but works other standard Arduinos as well)
        1 LCD 16*2
        1 10 kilo ohm potentiometer
        2 220 ohm resistors
        2 push buttons
        connecting wires
    Circuit connections:
        The connections are available in test.png
        The LCD pins used are rs:12, en:11, d4:5, d5:4, d6:9, d7:8
        The interrupts attached are pins 2 and 3 respectively each connected to a push button
        Pontentiometer is connected to the contrast of the LCD

Author:
    Name: M Naveen Kumar
    Github: mnaveenkumar2009
*/

#include <LiquidCrystal.h>
#define PM pinMode
#define DR digitalRead
#define f(i,n) for(i=0;i<n;i++)
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 9, d7 = 8;//LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int isprime[100];//stores 0 if the index is not a prime otherwise 1
int number=0,prevnumber=0,score=0;//the user scores and the random number generated details

//check1 is called when pushbutton 2 is pressed indicating the user claims the number to be a prime number
void check1(){
  lcd.setCursor(0,1);
  if(isprime[number]){
    score++;
    lcd.print("Correct");
  }
  else{
    lcd.print("Wrong  ");
  }
  number=rand()%100;
}
//check2 is called when pushbutton 3 is pressed indicating the user claims the number to be a composite number
void check2(){
  lcd.setCursor(0,1);
  if(!isprime[number]){
    score++;
    lcd.print("Correct");
  }
  else{
    lcd.print("Wrong  ");
  }
  number=rand()%100;
}

//seive sets the isprime array to the corresponding value depending on whether it is prime or not from 1 to 100 in O(NlogN time) (where N is 100)
void seive(){
  int i,j;
  f(i,100)
  {
   	isprime[i]=1;//set all numbers as prime
  }
  //set 1 and 0 as not prime
  isprime[0]=0;
  isprime[1]=0;
  
  f(i,98){
    if(isprime[i+2])
    {   //setting all multiples of the prime number to not a prime excluding the number itself
        f(j,(100/(i+2))-1){
            isprime[(i+2)*(j+2)]=0;
        }
    }
  } 
}
void setup() {
  lcd.begin(16, 2);
  PM(2,INPUT);
  PM(3,INPUT);
  seive();
  number=rand()%100;
  lcd.setCursor(0, 0);
  lcd.print(number);
  
  attachInterrupt(digitalPinToInterrupt(2), check1, RISING);
  attachInterrupt(digitalPinToInterrupt(3), check2, RISING);
}

void loop() {
  //print next number once the interrupt is called and the number has changed
  if(prevnumber!=number){
    delay(500);
  	lcd.setCursor(0,0);
  	prevnumber=number;  
  	lcd.clear();
  	lcd.print(number);
    lcd.setCursor(0,1);
    lcd.print("Score ");
    lcd.print(score);
  }
}