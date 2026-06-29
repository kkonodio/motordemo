#ifndef __SENSOR_H__
#define __SENSOR_H__
#include <reg52.h>
#define IR_PIN           P3^3 
#define IR_ACTIVE_LEVEL  0
#define IR_DEBOUNCE_MS   2  
extern unsigned int idata ir_count; 
void ir_init(void); 
void ir_ISR(void); 
#endif