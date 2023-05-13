#include "inits.h"
#include "pca9685.h"
#include "joystick.h"
#include "movement.h"


void main(void){
    initUC();
    pca9685_init(0x00, 50);
    delay_ms(5);
    initPWMandMotor();
    ps2_init();
    
    joystick_config();//called 3 times to enable analog sticks
    joystick_config();
    joystick_config();
    
    #asm("sei")
    while (1){
        joystick_poll();
        setMotorSpeed(data5, data3);
        setServos();
        
    }
}
