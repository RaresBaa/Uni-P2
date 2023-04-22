#include "inits.h"
#include "pca9685.h"
#include "joystick.h"

void main(void){
int8_t angle = 0;
    initUC();
    pca9685_init(0x00, 50);
    delay_ms(5);
    ps2_init();
    
    joystick_config();
    joystick_config();
    joystick_config();
    
    pca9685_servo(0,0);
    
    #asm("sei")
    while (1){
        joystick_poll();
        putchar(' ');
        putchar(data2);
        putchar(' ');
        putchar(data3);
        putchar('\n');
        /*
        pca9685_servo(0, 0); // set servo0 position to 0 degrees
        for (angle = -90; angle <= 90; angle += 5) {
            pca9685_servo(0, angle);
            delay_ms(25);
        }
        for (angle = 90; angle >= -90; angle -= 5) {
            pca9685_servo(0, angle);
            delay_ms(25);
        }          
        */
    }
}
