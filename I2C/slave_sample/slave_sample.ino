#include <compat/twi.h>

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
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
 
  // 0) init TWI @ 100 kHz
  TWBR = ((F_CPU / 100000L) - 16) / 2;
  
  // 1) load slave address and enable ack
  TWAR = SLA << 1;
  TWCR = (1 << TWEA) | (1 << TWEN) | (1<<TWIE);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}

ISR(TWI_vect) {		// TWI interrupt service routine
  switch(TW_STATUS){
    case TW_SR_SLA_ACK:
      // 2) receive byte
  	  TWCR = (1 << TWEA) | (1 << TWEN) | (1<< TWINT) | (1<<TWIE);
 	  break;
    
    case TW_SR_DATA_ACK:
  	  getData = TWDR;
      digitalWrite(led1, getData & 1);
  	  digitalWrite(led2, (getData >> 1) & 1);

      // 3) stop receiving: send nack for next receiving bytes
  	  TWCR = (1 << TWEN) | (1<< TWINT) | (1<<TWIE);
      break;
    
    case TW_SR_STOP:
      // 4) release bus
  	  TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT) | (1<<TWIE);
	  break;
    
    case TW_ST_SLA_ACK:
  	  // 5) send  byte
      sendData = 0;
  	  if (!digitalRead(button1)) sendData |= 1;
  	  if (!digitalRead(button2)) sendData |= 1<<1;
  	  TWDR = sendData;
  	  TWCR = (1 << TWEN) | (1 << TWINT) | (1<<TWIE);
	  break;
    
    case TW_ST_DATA_NACK:
      // 6) release bus
  	  TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT) | (1<<TWIE); 
	  break;
    
    default:
      // 9) check for errors and if one occurs send stop condition
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE)| (1 << TWEA);
      digitalWrite(errorLed, HIGH);
  }
}