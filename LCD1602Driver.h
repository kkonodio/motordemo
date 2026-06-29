#ifndef __LCD1602DRIVER_H__
#define __LCD1602DRIVER_H__

typedef struct{
	void (*init)(void);
	void (*showstr)(unsigned char Row, unsigned char Col, unsigned char *Str);
	
	void (*shownum)(unsigned char Row, unsigned char Col, unsigned int Number,unsigned int Len)reentrant;

}LCD_Display;

extern LCD_Display lcd;

void lcd_init(void);

#endif