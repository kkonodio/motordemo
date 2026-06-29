#include<reg52.h>
#include<Encoder.h>


#define ENCODER_PPR 360

sbit ENCODER_A = P3^2;
sbit ENCODER_B = P1^5;
sbit ENCODER_Z = P3^4;
/*  

     rpm = 60 * pulse / (ENCODER_PPR *  period of timer)  
	 
*/
static unsigned int min_div_ppr = 60 * 2 * 100 / ENCODER_PPR;

static volatile unsigned int raw_pulse;
static volatile unsigned int abs_pulse;
static volatile unsigned char timer_tick;

Encoder encoder;

static void _init(){
	ENCODER_A = 1;
	ENCODER_B = 1;
	ENCODER_Z = 1;
	
	raw_pulse  = 0;
	abs_pulse  = 0;
	timer_tick = 0; 
	
	encoder.pulse   = 0;
	encoder.rpm100  = 0;
	encoder.dir     = ENCO_DIR_CW;
	
	
	/*timer1 init*/
	TMOD &= 0xF0;
	TMOD |= 0x01;
	/*50ms*/
	TH0   = 0x3C;    
	TL0   = 0xB0;
	ET0   = 1;
	TR0   = 1;
	
	/*int0 init*/
	IT0 = 1;
	EX0 = 1;
	
	EA  = 1;
}

unsigned int _get_speed(){
	return encoder.rpm100;
}

unsigned int _get_pulse(){
	return encoder.pulse;
}


static void _reset(){
	encoder.rpm100   = 0;
}

void encoder_init(){
	encoder.get_speed   = _get_speed;
	encoder.get_pulse   = _get_pulse;
	encoder.reset       = _reset;
	
	_init();
}

void encoder_A_IRQ(void)interrupt 0
{
	if(ENCODER_B == 0){
		encoder.dir = ENCO_DIR_CCW;
		if(raw_pulse < 0xffff)raw_pulse++;
	}
	if(ENCODER_B == 1){
		encoder.dir = ENCO_DIR_CW;
		if(raw_pulse > 0)raw_pulse--;
	}
	if(abs_pulse < 0xffff)abs_pulse++;
}

void timer1_IRQ()interrupt 1
{
	TH0   = 0x3C;
	TL0   = 0xB0;
	timer_tick++;
	if(timer_tick >= 10){
		timer_tick       = 0;
		encoder.pulse    = abs_pulse;

		encoder.rpm100   = min_div_ppr * abs_pulse;
		abs_pulse        = 0;
	}
}