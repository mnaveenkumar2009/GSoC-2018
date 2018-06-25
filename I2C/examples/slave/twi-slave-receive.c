#include <compat/twi.h>
#define s(i) Serial.println(i)
#define f(i,n) for(i=0;i<n;i++)
int SLA = 4;
int rbl=0,tbl=0,ri=0,ti=0,i;
int twi_transmit_buffer[32],TX[32];
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

void twi_reply(int ack)
{
  if(ack){
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
  }else{
	  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
  }
}


void on(){
  TWBR = ((F_CPU / 100000L) - 16) / 2;
  TWAR = SLA << 1;
}
void receive (int nob){
 	rbl=nob;
  	ri=0;
  	TWCR = (1 << TWEA) | (1 << TWEN) | (1<<TWIE) | (1<<TWINT);
}
void send(int nob){
	tbl=nob;
  	ti=0;
  	TWCR = (1 << TWEA) | (1 << TWEN) | (1<<TWIE) | (1<<TWINT);
}
void setup() {
  	Serial.begin(9600);
 	on();
  f(i,6)twi_transmit_buffer[i]=0;
  TWCR = (1 << TWEA) | (1 << TWEN) | (1<<TWIE) | (1<<TWINT);
}

void loop() {  
  delay(300);
}

ISR(TWI_vect) {
  print_stat(TW_STATUS);
  int rec=0,sen=0;
  switch(TW_STATUS){
    case TW_SR_SLA_ACK:   // addressed, returned ack
    case TW_SR_GCALL_ACK: // addressed generally, returned ack
    case TW_SR_ARB_LOST_SLA_ACK: 
    case TW_SR_ARB_LOST_GCALL_ACK: 
      receive(5);
      break;
    case TW_SR_DATA_ACK:       // data received, returned ack
    case TW_SR_GCALL_DATA_ACK:
      if(ri < rbl){
        twi_transmit_buffer[ri++] = TWDR;
        twi_reply(1);
      }else{
        s("send na");
        twi_reply(0);
      }
      break;
    case TW_SR_STOP: 
    
   	 	f(i,6){
          s(twi_transmit_buffer[i]);
    	}s("bye");
      	ri = 0;
    	rbl = 0;
    	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);
      break;
    case TW_SR_DATA_NACK:
    case TW_SR_GCALL_DATA_NACK:
      twi_reply(0);
      break;
    default:
    	s(6);
     	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE)| (1 << TWEA);
  }
}