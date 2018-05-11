#include <compat/twi.h>

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
  
  // 0) init TWI @ 100 kHz
  TWBR = ((F_CPU / 100000L) - 16) / 2;
  unsigned char temp = TWCR;
  delay(2); // wait for arduino micro to init;
}

// the loop routine runs over and over again forever:
void loop() {
  // 1) start condition
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
  delay(300);
}

ISR(TWI_vect) {	// TWI interrupt service routine
  switch(TW_STATUS){
    case TW_START:
      // 2) send SLA+W
      TWDR = (SLA << 1) | TW_WRITE;
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
      break;
    
    case TW_MT_SLA_ACK:
      sendData = 0;
  	  if (!digitalRead(button1)) sendData |= 1;
      if (!digitalRead(button2)) sendData |= 1<<1;

      // 3) send data
      TWDR = sendData;
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
	  break;
    
    case TW_MT_DATA_ACK:
      // 4) send repeated start condition 
  	  TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTA) | (1<<TWIE);
	  break;
    
    case TW_REP_START: 
      // 5) send SLA+R
      TWDR = (SLA << 1) | TW_READ;
      TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
	  break;
    
    case TW_MR_SLA_ACK:
      // 6) get only one byte, send nack
  	  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
      break;
    
    case TW_MR_DATA_NACK:
      // 7) receive data and updata leds
      getData = TWDR;
      digitalWrite(led1, getData & 1);
      digitalWrite(led2, (getData >> 1) & 1);
	  
      // 8) send stop condition
      TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO) | (1<<TWIE);
      break;
    
    default:
      // 9) check for errors and if one occurs send stop condition
      TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO) | (1<<TWIE);
      digitalWrite(errorLed, HIGH);
  }
}