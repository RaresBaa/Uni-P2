#define PSclock            2               // PD2
#define PSdata             3               // PD3
#define PSacknolage  4               // PD4
#define PScommand   5               // PD5
#define PSattention     6               // PD6

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
                PORTD|=(1<<PScommand);//sbi(PORTD, PScommand);
                }// bit bang "command" out on PScommand wire.
                else
                {
                PORTD&=~(1<<PScommand);
                }
                PORTD&=~(1<<PSclock);                             // CLOCK LOW
                delay_us(1);                                              // wait for output to stabilise
                if((PIND & _BV(PSdata)))
                {
                    data|=(1<<i);  // read PSdata pin and store
                }//else cbi(data, i);
                PORTD|=(1<<PSclock);                             // CLOCK HIGH
        }
        PORTD|=(1<<PScommand);

        delay_us(20);                                                   // wait for ACK to pass.

        return(data);
}

void joystick_config()
{
     // put controller in config mode
       PORTD|=(1<<PScommand);
       PORTD|=(1<<PSclock);
       PORTD&=~(1<<PSattention);

       gameByte(0x01);
       gameByte(0x43);
       gameByte(0x00);
       gameByte(0x01);
       gameByte(0x00);

       PORTD|=(1<<PScommand);
       delay_ms(1);
       PORTD|=(1<<PSattention);

       delay_ms(10);

       // put controller in analouge mode
       PORTD|=(1<<PScommand);
       PORTD|=(1<<PSclock);
       PORTD&=~(1<<PSattention);

       gameByte(0x01);
       gameByte(0x44);
       gameByte(0x00);
       gameByte(0x01);
       gameByte(0x03);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);

       PORTD|=(1<<PScommand);
       delay_ms(1);
       PORTD|=(1<<PSattention);

       delay_ms(10);

       // exit config mode
       PORTD|=(1<<PScommand);
       PORTD|=(1<<PSclock);
       PORTD&=~(1<<PSattention);

       gameByte(0x01);
       gameByte(0x43);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x5A);
       gameByte(0x5A);
       gameByte(0x5A);
       gameByte(0x5A);
       gameByte(0x5A);

       PORTD|=(1<<PScommand);
       delay_ms(1);
       PORTD|=(1<<PSattention);

       delay_ms(10);

       // poll controller and check in analouge mode.
       PORTD|=(1<<PScommand);
       PORTD|=(1<<PSclock);
       PORTD&=~(1<<PSattention);

       gameByte(0x01);
       chk_ana = gameByte(0x42);            // the 2nd byte to be returned from the controller should = 0x73 for "red" analouge controller.
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);
       gameByte(0x00);

       PORTD|=(1<<PScommand);
       delay_ms(1);
       PORTD|=(1<<PSattention);
       delay_ms(10);

}

void ps2_init()
{
    DDRD|=(1<<PD2);                 // clock. output. (blue)

   DDRD&=~(1<<PD3);                 // data. input. (brown)
   PORTD|=(1<<PD3);               //    enable pullup resistor

   DDRD&=~(1<<PD4);                 // acknolage. input. (green)
   PORTD|=(1<<PD4);               //    enable pullup resistor

   DDRD|=(1<<PD5);                 // command. output. (orange)

   DDRD|=(1<<PD6);                 // attention. output. (yellow)
}

void joystick_poll()
{
        PORTD|=(1<<PScommand);                          // start communication with PSx controller
        PORTD|=(1<<PSclock);
        PORTD&=~(1<<PSattention);

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
        PORTD|=(1<<PScommand);                      // close communication with PSx controller
        delay_us(1);
        PORTD|=(1<<PSattention);                        // all done.
}