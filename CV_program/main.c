#include <delay.h>
#include<io.h>
#include <i2c.h>
#include "inits.h"
#include "joystick.h"

void timer0_init(uint8 duty){ //TOP value 0xFF
	OCR0A = (duty*0.25); // duty cycle formula : (OCR0A/256)*100 in procente
                        // PWM output on  PB3 si PB4
}
void timer2_init(uint16_t duty_cycle) { //TOP value 0xFF
    OCR2A = (duty_cycle*0.25);  // Set the PWM duty cycle
                                // duty cycle formula : (OCR0A/256)*100 in procente
                                // PWM output on  PD7
}

void timer1_PWM(uint16_t PWM_freq,uint16_t val) { // from 0 to 65535 or 16 bit values
    ICR1H = (PWM_freq >> 8); //since ICR1 and OCR1B are 16 bit register broken into 8 bit parts split the values for the OCR1B and ICR1 in two parts
    ICR1L = PWM_freq;        // duty cycle formula : (OCR1B/TOP)*100 in procente
    OCR1BH = (val >> 8);    // PWM_freq = F_CPU/ICR1+1
    OCR1BL = val;
}

// Send text over serial for USART0
void sendText(char *tmp){ // think arduino serial.print() function for USART0
    int i;
    for(i = 0;i<strlen(tmp);i++){
        while ( !( UCSR0A & (1<<UDRE0)) );
        UDR0 = tmp[i];
    }
}

void main(void){
    init_io();
    i2c_init();//I2C: SDA PORTA0, SCK PORTA1


    #asm("sei")// Global enable interrupts
    while (1){
          

    }
}
