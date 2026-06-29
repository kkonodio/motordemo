#include<reg52.h>
#include<LCD1602Driver.h>

sbit LCD_RS = P2^6;
sbit LCD_WR = P2^5;
sbit LCD_E  = P2^7;

#define LCD_DataPort P0

LCD_Display lcd;

static void LCD_Delay(){
	unsigned char i,j;
	
	i = 2;
	j = 239;
	
	do{
		while(--j);
	}while(--i);
}

static void LCD_WriteCommand(unsigned char Command){
	LCD_RS = 0;
	LCD_WR = 0;
	LCD_DataPort = Command;
	LCD_E = 1;
	LCD_Delay();
	LCD_E = 0;
	LCD_Delay();
}
	
static void LCD_WriteData(unsigned Data){
	LCD_RS = 1;
	LCD_WR = 0;
	LCD_DataPort = Data;
	LCD_E = 1;
	LCD_Delay();
	LCD_E = 0;
	LCD_Delay();
}

static void _init(){
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x0c);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x01);
}

static void LCD_SetCursor(unsigned char Row, unsigned char Col){
	
	if(Row == 1){
		LCD_WriteCommand(0x80|(Col - 1));
	}
	else{
		LCD_WriteCommand(0x80|(Col - 1) + 0x40);
	}
}

static void LCD_ShowString(unsigned char Row, unsigned char Col, unsigned char *Str){
	
	unsigned char i;
	
	LCD_SetCursor(Row, Col);
	
	for(i = 0; Str[i] != '\0'; i++){
		LCD_WriteData(Str[i]);
	}
	
}

static int LCD_Pow(int x, int y){
	unsigned int i;
	int result = 1;
	for(i = y; y > 0; y--){
		result *= x;
	}
	return result;
}

static void LCD_ShowNum(unsigned char Row, unsigned char Col, unsigned int Number,unsigned int Len)reentrant{
	int i;
	LCD_SetCursor(Row,Col);

	for(i = Len; i > 0; i--){
		LCD_WriteData('0' + Number/LCD_Pow(10,i - 1)%10);
	}
}



void lcd_init(){
	lcd.init    = _init;
	lcd.showstr = LCD_ShowString;
	lcd.shownum = LCD_ShowNum;

	_init();
}

