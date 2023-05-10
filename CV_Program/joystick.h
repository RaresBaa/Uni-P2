#define PSclock            3              // PC2
#define PSdata             5               // PC5
#define PSacknolage  6               // PC6
#define PScommand   4              // PC4
#define PSattention     2               // PC2

#define _BV(bit) (1<<(bit))
unsigned char chk_ana = 0;
int data0=0,data1=0,data2=0,data3=0,data4=0,data5=0,temp=0;

int gameByte(int command)
{
        int i ;// clear data variable to save setting low bits later.
        int data = 0x00;
        delay_us(1);
        for(i=0;i<8;i++)
        {
                if(command & _BV(i))
                {
                PORTC|=(1<<PScommand);//sbi(PORTC, PScommand);
                }// bit bang "command" out on PScommand wire.
                else
                {
                PORTC&=~(1<<PScommand);
                }
                PORTC&=~(1<<PSclock);                             // CLOCK LOW
                delay_us(1);                                              // wait for output to stabilise
                if((PINC & _BV(PSdata)))
                {
                    data|=(1<<i);  // read PSdata pin and store
                }//else cbi(data, i);
                PORTC|=(1<<PSclock);                             // CLOCK HIGH
        }
        PORTC|=(1<<PScommand);

        delay_us(20);                                                   // wait for ACK to pass.

        return(data);
}

void joystick_config()
{
     // put controller in config mode
       PORTC|=(1<<PScommand);
       PORTC|=(1<<PSclock);
       PORTC&=~(1<<PSattention);

       gameByte(0x01);
       gameByte(0x43);
       gameByte(0x00);
       gameByte(0x01);
       gameByte(0x00);

       PORTC|=(1<<PScommand);
       delay_ms(1);
       PORTC|=(1<<PSattention);

       delay_ms(10);

       // put controller in analouge mode
       PORTC|=(1<<PScommand);
       PORTC|=(1<<PSclock);
       PORTC&=~(1<<PSattention);

       gameByte(0x01);
       gameByte(0x44);
       gameByte(0x00);
       gameByte(0x01);
       gameByte(0x03);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);

       PORTC|=(1<<PScommand);
       delay_ms(1);
       PORTC|=(1<<PSattention);

       delay_ms(10);

       // exit config mode
       PORTC|=(1<<PScommand);
       PORTC|=(1<<PSclock);
       PORTC&=~(1<<PSattention);

       gameByte(0x01);
       gameByte(0x43);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x5A);
       gameByte(0x5A);
       gameByte(0x5A);
       gameByte(0x5A);
       gameByte(0x5A);

       PORTC|=(1<<PScommand);
       delay_ms(1);
       PORTC|=(1<<PSattention);

       delay_ms(10);

       // poll controller and check in analouge mode.
       PORTC|=(1<<PScommand);
       PORTC|=(1<<PSclock);
       PORTC&=~(1<<PSattention);

       gameByte(0x01);
       chk_ana = gameByte(0x42);            // the 2nd byte to be returned from the controller should = 0x73 for "red" analouge controller.
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);

       PORTC|=(1<<PScommand);
       delay_ms(1);
       PORTC|=(1<<PSattention);
       delay_ms(10);

}

void ps2_init()
{
    DDRC|=(1<<DDRC.3);                 // clock. output. (blue)

   DDRC&=~(1<<DDRC.5);                 // data. input. (brown)
   PORTC|=(1<<PORTC.5);               //    enable pullup resistor

   DDRC&=~(1<<DDRC.6);                 // acknolage. input. (green)
   PORTC|=(1<<PORTC.6);               //    enable pullup resistor

   DDRC|=(1<<DDRC.4);                 // command. output. (orange)

   DDRC|=(1<<PORTC.2);                 // attention. output. (yellow)
}

void joystick_poll()
{
        PORTC|=(1<<PScommand);                          // start communication with PSx controller
        PORTC|=(1<<PSclock);
        PORTC&=~(1<<PSattention);

        gameByte(0x01);                                       // bite 0. header.
        temp = gameByte(0x42);                          // bite 1. header. (should possibly put test on this byte to detect unplugging of controller.)
        gameByte(0x00);                                       // bite 2. header.

        data0 = gameByte(0x00);                         // bite 3. first data bite.
        data1 = gameByte(0x00);                         // bite 4.
        data2 = gameByte(0x00);                         // bite 5.
        data3 = gameByte(0x00);                         // bite 6.
        data4 = gameByte(0x00);                         // bite 7.
        data5 = gameByte(0x00);                         // bite 8.

        delay_us(1);
        PORTC|=(1<<PScommand);                      // close communication with PSx controller
        delay_us(1);
        PORTC|=(1<<PSattention);                        // all done.
}

//function to print to serial all the data from the joystick, used to determine what every data pin does
void testJoystickData(void){
        printf("\n\rAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa\n\r");
        printf("%x %x %x %x", data2, data3, data4, data5);
        switch(data0){
            case 0xef://D-Pad up
                printf("D-pad up\n\r");
                break;
            case 0xdf://D-Pad right
                printf("D-pad right\n\r");
                break;
            case 0x7f://D-pad left
                printf("D-pad left\n\r");
                break;
            case 0xbf://D-pad down
                printf("D-pad down\n\r");
                break;
            case 0xfd://Analog Stick left
                printf("Analog Stick left\n\r");
                break;
            case 0xfb://Analog Stick right
                printf("Analog Stick right\n\r");
                break;
            case 0xf7://Start  
                printf("Start\n\r");
                break;
        }
        switch(data1){
            case 0x7f: //square
                printf("Square\n\r");
                break;
            case 0xef: //triangle
                printf("Triangle\n\r");
                break;
            case 0xdf: //circle
                printf("Circle\n\r");
                break;
            case 0xbf: //cross
                printf("Cross\n\r");
                break;
            case 0xfb: //L1
                printf("L1\n\r");
                break;
            case 0xf7: //R1
                printf("R1\n\r");
                break;
            case 0xfe: //L2
                printf("L2\n\r");
                break;
            case 0xfd: //R2
                printf("R2\n\r");
                break;
        }
}