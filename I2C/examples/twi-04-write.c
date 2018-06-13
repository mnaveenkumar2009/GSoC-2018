#include <compat/twi.h>
int SLA;
int count=1;
int TXBuffLen=0;
volatile int TXBuffIndex=0;
int requestmode=-1;
int errorcode=1;
int twi_transmit_buffer[32];
int i;
void I2C_REQUEST_SEND(int address,int buffer[5],int n_byte){
    requestmode=0;// write mode
    SLA=address;
    for(i =0;i<n_byte;i++)
      twi_transmit_buffer[i] = buffer[i];
    TXBuffLen=n_byte; 
  	TXBuffIndex=0;
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
}
void setup()
{
TWBR = ((F_CPU / 100000L) - 16) / 2;
  Serial.begin(9600);
}
void loop(){
  int sendData[5] = {'h','e','l','l','o'};
            Serial.println(2);
    I2C_REQUEST_SEND(4,sendData,5);
  delay(300);
}

ISR(TWI_vect){
  switch(TW_STATUS){
    
      case TW_START:
          errorcode = 0;
          if(requestmode!=-1){
            Serial.println(1);
            TWDR = (SLA << 1) | (requestmode); // SLA + Read/Write
            requestmode=-1;
          }
          TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE)|(1<<TWEA); // Continue Transfer
          break;
      
      case TW_MT_SLA_ACK:
    	case TW_MT_DATA_ACK:
          if(TXBuffIndex<TXBuffLen){
            Serial.println(3);
            Serial.println("vd");
            Serial.println(twi_transmit_buffer[TXBuffIndex]);
            TWDR = twi_transmit_buffer[TXBuffIndex++];
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
          }
          else{
            Serial.println(4);
              TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWSTO);
          }
          break;
      case TW_REP_START: 
    		Serial.println("897");
      		TWDR = (SLA << 1) | TW_WRITE;
    		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
    		break;
      case TW_MT_SLA_NACK:
      case TW_MT_DATA_NACK:
            Serial.println(5);
          errorcode=2;// in case of slave not connected or receiving NACK from slave
          break;
      default:
          errorcode=3;
          TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO) | (1<<TWIE);//default case sends stop condition
    }
}