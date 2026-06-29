#include<reg52.h>
#include<MotorDriver.h>
#include<common.h>

#define MAX_DELAY 64835
#define MIN_DELAY 50535

#define RPM_MIN 100
#define RPM_MAX 2000

#define STEP_US_MAX 15000
#define STEP_US_MIN 700

#define MAX_STEPS 60000

sbit MOTOR_IN1 = P1^0;   /* blue   */
sbit MOTOR_IN2 = P1^1;   /* pink   */
sbit MOTOR_IN3 = P1^2;   /* yellow */
sbit MOTOR_IN4 = P1^3;   /* orange */

Motor motor;



static const unsigned char code phase_table[8] = {
    0x01,   /* PHASE_0 */
    0x03,   /* PHASE_1 */
    0x02,   /* PHASE_2 */
    0x06,   /* PHASE_3 */
    0x04,   /* PHASE_4 */
    0x0C,   /* PHASE_5 */
    0x08,   /* PHASE_6 */
    0x09,   /* PHASE_7 */
};

static void _output_phase(){
	unsigned char pt = phase_table[motor.phase];
	MOTOR_IN1 = (pt >> 0) & 1;
	MOTOR_IN2 = (pt >> 1) & 1;
	MOTOR_IN3 = (pt >> 2) & 1;
	MOTOR_IN4 = (pt >> 3) & 1;
}


static void _init(void){
	MOTOR_PORT &= 0xf0;
	
	motor.target_steps      = 0;
	motor.current_position  = 0;
	motor.step_delay        = 0;
	motor.phase             = 0;
	motor.rpm100            = 0;
	
	motor.dir               = MOTOR_DIR_CW;
	motor.run               = STOP;
	
	motor.step_delay        = 65535 - 14648;    //default 1 rpm
	
	TMOD &= 0x0F;
	TMOD |= 0x10;
//	TMOD &= 0xF0;
//	TMOD |= 0x01;
	ET1   = 1;
	TR1   = 0;
    EA    = 1;	
}


static void _set_speed_2f(unsigned int rpm100){
	unsigned int stepus;                  /*14648 to 732 */
	if(rpm100 == 0){
		motor.stop();
		return;
	}
	if(rpm100 <= RPM_MIN)   rpm100 = RPM_MIN;
	if(rpm100 >= RPM_MAX)   rpm100 = RPM_MAX;
	
	motor.rpm100 = rpm100;
	stepus  = 1464800UL / rpm100;                	/*   60*1000000 / STEP_PER_REV = 14648 */
	
	if(stepus <= STEP_US_MIN)     stepus = STEP_US_MIN;
	if(stepus >= STEP_US_MAX)     stepus = STEP_US_MAX;
	
	stepus  = 0xffff - stepus + 1;
	motor.set_delay(stepus);
}

static void _set_dir(Motor_Dir dir){
	motor.dir = dir;
}

static void _set_steps(unsigned int steps){
	if(steps >= MAX_STEPS) steps = MAX_STEPS;
	motor.target_steps  = steps;
}

static void _set_delay(long delay){
	if(delay < MIN_DELAY)  delay = MIN_DELAY;
	if(delay > MAX_DELAY)  delay = MAX_DELAY;
	
	motor.step_delay = delay;
}

static void _move(){
	motor.run           = RUN;
	
	TH1  = (motor.step_delay >> 8) & 0xff;
	TL1  = motor.step_delay        & 0xff;
	TR1  = 1;
}

static void _stop(void){
	TR1 = 0;
	motor.run          = STOP;
	motor.target_steps = 0;
	MOTOR_PORT        &= 0xF0;
}

void motor_init(){
	motor.set_speed = _set_speed_2f;
	motor.set_dir   = _set_dir;
	motor.set_step  = _set_steps;
	motor.set_delay = _set_delay;
	motor.move      = _move;
	motor.stop      = _stop;
	
	_init();
}

void motor_timerIRQ() interrupt 3
{
	TH1  = (motor.step_delay >> 8) & 0xff;
	TL1  = motor.step_delay        & 0xff;
	if(motor.run == STOP){
		TR1 = 0;
		return;
	}	
	
	motor.target_steps--;	
	if(motor.dir == MOTOR_DIR_CW){
		motor.phase          = (motor.phase + 1) % 8;
		motor.current_position++;
	}else{
		motor.phase          = (motor.phase - 1) % 8;
		motor.current_position--;
	}
	_output_phase();
	
	if(motor.target_steps == 0){
		TR1 = 0;
		motor.run = STOP;
		MOTOR_PORT &= 0xF0;  
	}
}



