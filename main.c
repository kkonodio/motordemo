#include<reg52.h>
#include<common.h>
#include<LCD1602Driver.h>
#include<MotorDriver.h>
#include<Encoder.h>
#include<PID.h>
#include<SpeedController.h>
#include<Key.h>
#include<Sensor.h>

void main(){
	int tick_10ms = 0;
	int tar = 0;
	int act = 0;
	unsigned char key = 0;
	unsigned char dir= motor.dir;
	
	ir_init();
	key_init();
	lcd_init();
	motor_init();
	encoder_init();
	speed_pid_init();
	speedcontroller_init();
	
	
	lcd.showstr(1,1,"tar");
	lcd.showstr(2,1,"act");
	lcd.showstr(1,5,"10.00");
	lcd.showstr(2,5,"00.00");
	
	lcd.showstr(1,11,"C:0000");
	lcd.showstr(2,16,"0");
	motor.set_dir(MOTOR_DIR_CCW);
	speedctl.set_target(1000);
	speedctl.start();

	while(1){
		lcd.shownum(1,13,ir_count,4);
        Delayms(10);

        tick_10ms++;
		
		key = key_read();
		if (key == 1) {
			lcd.showstr(2,15,"1");
			speedctl.able = !speedctl.able;
			if (speedctl.able) speedctl.start();
			else               speedctl.stop();
		}
		if (key == 2) {
			lcd.showstr(2,15,"2");
			speedctl.target_rpm += 100;
			speedctl.target_rpm %= 1000;
			speedctl.target_rpm += 1000;
		}
		
        if (tick_10ms % 30 == 0) {
			speedctl.update();
			tar = speedctl.target_rpm;
			act = speedctl.actual_rpm;
			lcd.shownum(1, 5, tar/100, 2);
			lcd.shownum(2, 5, act/100, 2);
			lcd.shownum(1, 8, tar%100, 2);
			lcd.shownum(2, 8, act%100, 2);
            tick_10ms = 0;
        } 
		
	}
}
