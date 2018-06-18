#include <compat/twi.h>
#define s(i) Serial.println(i)
#define f(i,n) for(i=0;i<n;i++)
int SLA = 4;
int rbl=0,tbl=0,ri=0,ti=0,i;
int RX[32],TX[32];
void print_stat(int x){
  switch(x){
  	case 0x08: s("TW_START");
            break;
 
  case 0x10: s("TW_REP_START");
              break;

  case 0x18: s("TW_MT_SLA_ACK");
              break;

  case 0x20: s("TW_MT_SLA_NACK");
              break;

  case 0x28: s("TW_MT_DATA_ACK");
              break;

  case 0x30: s("TW_MT_DATA_NACK");
              break;

  case 0x38: s("TW_MT_ARB_LOST");
              break;

  case 0x40: s("TW_MR_SLA_ACK");
              break;

  case 0x48: s("TW_MR_SLA_NACK");
              break;

  case 0x50: s("TW_MR_DATA_ACK");
              break;

  case 0x58: s("TW_MR_DATA_NACK");
              break;

  case 0xA8: s("TW_ST_SLA_ACK");
              break;

  case 0xB0: s("TW_ST_ARB_LOST_SLA_ACK");
              break;

  case 0xB8: s("TW_ST_DATA_ACK");
              break;

  case 0xC0: s("TW_ST_DATA_NACK");
              break;

  case 0xC8: s("TW_ST_LAST_DATA");
              break;

  case 0x60: s("TW_SR_SLA_ACK");
              break;

  case 0x68: s("TW_SR_ARB_LOST_SLA_ACK");
              break;

  case 0x70: s("TW_SR_GCALL_ACK");
              break;

  case 0x78: s("TW_SR_ARB_LOST_GCALL_ACK");
              break;

  case 0x80: s("TW_SR_DATA_ACK");
              break;

  case 0x88: s("TW_SR_DATA_NACK");
              break;

  case 0x90: s("TW_SR_GCALL_DATA_ACK");
              break;

  case 0x98: s("TW_SR_GCALL_DATA_NACK");
              break;

  case 0xA0: s("TW_SR_STOP");
              break;

  case 0xF8: s("TW_NO_INFO");
              break;

  case 0x00: s("TW_BUS_ERROR");
              break;
  }
}
void on(){
  TWBR = ((F_CPU / 100000L) - 16) / 2;
  TWAR = SLA << 1;
}
void receive (int nob){
 	rbl=nob;
  	ri=0;
  	TWCR = (1 << TWEA) | (1 << TWEN) | (1<<TWIE);
}
void send(int nob){
	tbl=nob;
  	ti=0;
  	TWCR = (1 << TWEA) | (1 << TWEN) | (1<<TWIE);
}
void setup() {
  	Serial.begin(9600);
 	on();
}

void loop() {
  f(i,5)TX[i]=i+1;
  send(4);
  delay(300);
  f(i,5)TX[i]=0;
  receive(4);
  delay(300);
}

ISR(TWI_vect) {
  print_stat(TW_STATUS);
  switch(TW_STATUS){
    case TW_SR_SLA_ACK:
    	if(ri<rbl){
        s("ri<rbl");
  	  	TWCR = (1 << TWEA) | (1 << TWEN) | (1<< TWINT) | (1<<TWIE);
      }
    	else{
        s("ri>=rbl");
        TWCR = (1 << TWEN) | (1<< TWINT) | (1<<TWIE);
      }
    	break;
    
    case TW_SR_DATA_ACK:
    	if(ri<rbl)
    	  {	
          s("ri<rbl");
              RX[ri++]=TWDR;
              s(TWDR);
          s("get");
            TWCR = (1 << TWEA) | (1 << TWEN) | (1<< TWINT) | (1<<TWIE);
        }
    	else{
          	s("ri>=rbl");
  	  		TWCR = (1 << TWEN) | (1<< TWINT) | (1<<TWIE);
    	}
      	break;
    
    case TW_SR_STOP:
  	  	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT) | (1<<TWIE);
	  	break;
    
    case TW_ST_SLA_ACK:
    case TW_ST_DATA_ACK:
    	if(ti<tbl)
        {	
          	s("ti<tbl");
          	TWDR =TX[ti++];
          	Serial.print("data : ");    
    		s(TWDR);
          	TWCR = (1 << TWEN) | (1 << TWINT) | (1<<TWIE) | (1<<TWEA);
        }
    	else{
    		s("ti>=tbl");
          	TWCR = (1 << TWEN) | (1 << TWINT) | (1<<TWIE);
        }
	  	break;
    case TW_ST_DATA_NACK:
  	  	TWCR = (1 << TWEN) | (1 << TWINT) | (1<<TWIE);
	  	break;
    case TW_SR_DATA_NACK:
    	TWCR = (1 << TWEN) | (1 << TWINT) | (1<<TWIE);
    	break;
    default:
    	s(6);
     	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE)| (1 << TWEA);
  }
}