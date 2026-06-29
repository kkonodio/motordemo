#ifndef __ENCODER_H__
#define __ENCODER_H__


typedef enum{
	ENCO_DIR_CW = 0,
	ENCO_DIR_CCW
}Encoder_Dir;

typedef struct {

    volatile unsigned int  pulse;    //pulse recived in 500ms
    volatile unsigned int  rpm100;   //rpm * 100               
	
	Encoder_Dir dir;  
    
	unsigned int  (*get_speed)(void);   
    unsigned int  (*get_pulse)(void);    
    void (*reset)(void);
} Encoder;

extern Encoder encoder;

void encoder_init(void);
void encoder_A_IRQ(void);
void timer1_IRQ(void);

#endif