#ifndef __PID_H__
#define __PID_H__

typedef struct{
	int kp;
	int ki;
	int kd;
	
	long integral;
	int last_err;
	
	void (*reset)(void);
	int  (*compute)(int target, int actual);
	
}PID;

extern PID speed_pid;

void speed_pid_init(void);

#endif