#include <reg52.h>
#include "Sensor.h"
sbit IR_SIGNAL = IR_PIN;
unsigned int idata ir_count;
static unsigned int idata last_timestamp;

static unsigned char  tl, th_second;
static unsigned char  th_first;
#define DEBOUNCE_TICKS  1500               

void ir_init(void)
{
	th_first = TH0;
    IR_SIGNAL = 1;
    ir_count        = 0;
    last_timestamp  = 0;
    IT1 = 1;
    EX1 = 1;             
}

void ir_ISR(void) interrupt 2
{
	unsigned int now;
    unsigned int delta;
    if (IR_SIGNAL != IR_ACTIVE_LEVEL)
        return;
	th_first  = TH0;
	tl        = TL0;
	th_second = TH0;
	if (th_first != th_second)
	{
		th_first = TH0;
		tl       = TL0;
	}
	
	now = ((unsigned int)th_first << 8) | tl;

	delta = now - last_timestamp;
    if (delta < DEBOUNCE_TICKS)return;
    last_timestamp = now;
    ir_count++;
}


//#include <reg52.h>
//#include <Sensor.h>
//sbit IR_SIGNAL = IR_PIN;
//unsigned int idata ir_count;
//static unsigned char idata filter_buf;
//static unsigned char idata confirm_cnt; 
//static bit last_state; 
//void ir_init(void) {
//    IR_SIGNAL    = 1;
//    ir_count     = 0;
//    filter_buf   = 0;
//    confirm_cnt  = 0;
//    last_state   = 1;
//}

//void ir_poll(void) {
//    unsigned char ones, temp, filtered;
//	
//	filter_buf <<= 1;
//	if(IR_SIGNAL == IR_ACTIVE_LEVEL)
//		filter_buf |= 1;
//	
//	temp = filter_buf;
//	ones = 0;
//	while(temp){
//		ones += temp & 1;
//		temp >>= 1;
//	}
//	
//	filtered = (ones <= 4);
//	
//	if(filtered != last_state){
//		confirm_cnt++;
//		if(confirm_cnt >= IR_CONFIRM_CNT){
//			last_state = filtered;
//			
//			if(filtered)ir_count++;
//			confirm_cnt = 0;
//		}
//	}else{
//		confirm_cnt = 0;
//	}
//}