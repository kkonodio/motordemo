#include<reg52.h>
#include<PID.H>

#define OUTPUT_MAXV 10000

#define ERROR_MAX 1000
#define INTEGRAL_MAX 10000
#define DELTA_MAX 500

#define P_TERM 1
#define I_TERM 1  
#define D_TERM 1

PID speed_pid;

static int _private_muti(int a, int b) {
    int result;
    result = a * b;           
    if (result >  OUTPUT_MAXV) result =  OUTPUT_MAXV;
    if (result < -OUTPUT_MAXV) result = -OUTPUT_MAXV;
    return (int)result;
}


static void _init(int Kp, int Ki, int Kd){
	speed_pid.kp = Kp;
	speed_pid.ki = Ki;
	speed_pid.kd = Kd;
	
	speed_pid.integral = -1000;
	speed_pid.last_err = 0;
}

static void _reset(){
	speed_pid.integral = 0;
	speed_pid.last_err = 0;
}

static int _compute(int target, int actual){
	int error;
	int p_term, i_term, d_term;
	int delta;
	int output;
	
	error = target - actual;
	if(error > ERROR_MAX) error = ERROR_MAX;
	if(error < -ERROR_MAX)error = -ERROR_MAX;
	
	speed_pid.integral += error;
	if(speed_pid.integral > INTEGRAL_MAX) speed_pid.integral = INTEGRAL_MAX;
	if(speed_pid.integral < -INTEGRAL_MAX)speed_pid.integral = -INTEGRAL_MAX;

	delta = error - speed_pid.last_err;
	if(delta > DELTA_MAX)delta = DELTA_MAX;
	if(delta < -DELTA_MAX)delta = -DELTA_MAX;
	speed_pid.last_err = error;

	
	p_term = _private_muti(speed_pid.kp, error);
	
	i_term = _private_muti(speed_pid.ki, speed_pid.integral);

	d_term = _private_muti(speed_pid.kd, delta);
	
	p_term = p_term >> 4;
	i_term = i_term >> 2;
	d_term = d_term >> 2;
	
	output = p_term + i_term + d_term;
	if(output > OUTPUT_MAXV)  output = OUTPUT_MAXV;
	if(output < -OUTPUT_MAXV) output = -OUTPUT_MAXV;

	return output;
}

void speed_pid_init(){
	
	speed_pid.reset   = _reset;
	speed_pid.compute = _compute;
	
	_init(P_TERM, I_TERM, D_TERM);
}