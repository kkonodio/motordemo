#ifndef __MOTORDRIVER_H__
#define __MOTORDRIVER_H__

#define MOTOR_PORT    P1
#define STEP_PER_REV  4096
#define STEP_MODE     8


typedef enum{
	MOTOR_DIR_CW  = 0,
	MOTOR_DIR_CCW = 1
}Motor_Dir;

typedef enum{
	STOP  = 0,
	RUN = 1
}Motor_Run;


typedef struct{
	
	unsigned int target_steps;
	int current_position;
	unsigned int step_delay;
	unsigned char phase;
	unsigned int rpm100;
	
	Motor_Dir dir;
	Motor_Run run;

	
	void (*set_speed)(unsigned int rpm);
	void (*set_dir)(Motor_Dir dir);
	void (*set_step)(unsigned int steps);
	void (*move)(void);
	void (*stop)(void);

	void (*set_delay)(long delay);
}Motor;

extern Motor motor;


void motor_init(void);
void motor_timerIRQ(void);

#endif