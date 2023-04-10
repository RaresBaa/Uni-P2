#include <delay.h>
#include<io.h>
#include <i2c.h>
#include "inits.h"
#include "joystick.h"

void main(void){
    init_io();
    i2c_init();//I2C: SDA PORTA0, SCK PORTA1


    #asm("sei")// Global enable interrupts
    while (1){
          

    }
}
