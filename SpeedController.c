#include<reg51.h>
#include<SpeedController.h>
#include<MotorDriver.h>
#include<Encoder.h>
#include<PID.h>

#define STEP_US_MAX 15000
#define STEP_US_MIN 700

#define RPM_MIN 100
#define RPM_MAX 2000

SpeedController speedctl;

static unsigned int _rpm100_to_delay_us(unsigned int rpm100)
{
    unsigned int delay_us;
	if(rpm100 < RPM_MIN)rpm100 = RPM_MIN;
	if(rpm100 > RPM_MAX)rpm100 = RPM_MAX;
    delay_us = (1464800UL / rpm100);
    return delay_us;
}

static void _init(){
	speedctl.target_rpm    = 0;
	speedctl.actual_rpm    = 0;
	speedctl.pid_out       = 0;
	speedctl.able          = DISABLED;
	
}

static void _set_target(unsigned int rpm100){
	if(rpm100 <= RPM_MIN)    rpm100 = RPM_MIN;
	if(rpm100 >= RPM_MAX)    rpm100 = RPM_MAX;
	speedctl.target_rpm = rpm100;
	if(!speedctl.able){
		speed_pid.reset();
	}
}

static void _start(){
	if(speedctl.target_rpm <= 0){motor.stop();return;}
	motor.set_speed(speedctl.target_rpm);
	motor.set_step(0xffff);
	motor.move();

	speedctl.able = ENABLED;

}

static void _zero_response(unsigned int rpm100){
	if(rpm100 <= RPM_MIN)    rpm100 = RPM_MIN;
	if(rpm100 >= RPM_MAX)    rpm100 = RPM_MAX;
	speedctl.target_rpm = rpm100;
	motor.set_speed(RPM_MIN);
	motor.set_step(0xffff);
	motor.move();
	
	speed_pid.reset();
	speedctl.able = ENABLED;
}

static void _stop(){
	motor.stop();
	speedctl.able    = DISABLED;
	speedctl.pid_out = 0;
	speedctl.actual_rpm = 0;
	speed_pid.reset();
}

static void _update(){
	int base_delay;
	long pid_delay;
	if(!speedctl.able)return;
	
	motor.set_step(0xff00);
	
	base_delay = _rpm100_to_delay_us(speedctl.target_rpm);
	
	speedctl.actual_rpm = encoder.get_speed();
	speedctl.pid_out    = speed_pid.compute(speedctl.target_rpm, speedctl.actual_rpm);
	pid_delay           = (long)base_delay - speedctl.pid_out;
	
	if(pid_delay > STEP_US_MAX) pid_delay = STEP_US_MAX;
	if(pid_delay < STEP_US_MIN) pid_delay = STEP_US_MIN;
	motor.set_delay(0xffff - pid_delay + 1);
}

void speedcontroller_init(){
	speedctl.set_target    = _set_target;
	speedctl.zero_response = _zero_response;
	speedctl.start         = _start;
	speedctl.stop          = _stop;
	speedctl.update        = _update;
	
	_init();
}