#ifndef __SPEED_CONTROLLER_H__
#define __SPEED_CONTROLLER_H__

typedef enum{
	DISABLED = 0,
	ENABLED
}SpeedctlAble;
typedef struct{
	unsigned int target_rpm;
	unsigned int actual_rpm;
	int pid_out;
	
	SpeedctlAble able;
	
	void (*set_target)(int rpm100);
	void (*zero_response)(unsigned int rpm100);
	void (*start)(void);
	void (*stop)(void);
	void (*update)(void);
}SpeedController;

extern SpeedController speedctl;

void speedcontroller_init(void);

#endif