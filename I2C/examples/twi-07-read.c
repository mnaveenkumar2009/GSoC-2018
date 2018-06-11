#include <compat/twi.h>
int SLA;
int count=1;
int RXBuffLen=0;
volatile int RXBuffIndex=0;
int requestmode=-1;
int errorcode=1;
int twi_receive_buffer[32];
int i;
void I2C_REQUEST_RECEIVE(int address,int buffer[5],int n_byte){
    requestmode=1;// read mode
    SLA=address;
    RXBuffLen=n_byte; 
  	RXBuffIndex=0;
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
    for(i =0;i<n_byte;i++)
        buffer[i] = twi_receive_buffer[i] ;
}
void setup()
{
pinMode(6,INPUT_PULLUP);
TWBR = ((F_CPU / 100000L) - 16) / 2;
  Serial.begin(9600);
}
void loop(){
  int getD[5];
    //Serial.println(2);
I2C_REQUEST_RECEIVE(4,getD,5);
  int i;
  for(i=0;i<5;i++)Serial.print((char)getD[i]);
  Serial.println("bye");
  delay(300);
}

ISR(TWI_vect){
  switch(TW_STATUS){
    
      case TW_START:
          errorcode = 0;
          if(requestmode!=-1){
           // Serial.println(1);
            TWDR = (SLA << 1) | (requestmode); // SLA + Read/Write
            requestmode=-1;
          }
          TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE); // Continue Transfer
          break;
      
      case TW_MR_SLA_ACK:
          if(RXBuffIndex<RXBuffLen){
           // Serial.println(3);
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE)|(1<<TWEA);
          }
          else{
            Serial.println(4);
              TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWSTO);
          }
          break;
    case TW_MR_DATA_ACK:
      // 4) send repeated start condition v
    		if(RXBuffIndex<RXBuffLen)
            {
              //Serial.println("99");
              //Serial.println("vd");
              twi_receive_buffer[RXBuffIndex++] = TWDR;
              //Serial.println(twi_receive_buffer[RXBuffIndex-1]);
             TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE)|(1<<TWEA);
            }
    		else
            {
          TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO) | (1<<TWIE);              
            }
	  break;
      case TW_REP_START: 
    		//Serial.println("897");
      		TWDR = (SLA << 1) | requestmode;
    		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE)|(1<<TWEA);
    		break;
      case TW_MR_SLA_NACK:
      case TW_MR_DATA_NACK:
           // Serial.println(5);
          errorcode=2;// in case of slave not connected or receiving NACK from slave
          break;
      default:
          errorcode=3;
          TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO) | (1<<TWIE);//default case sends stop condition
    }
}