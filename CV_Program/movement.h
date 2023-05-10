//PWM exapnder pin connections
#define ArmServo0 0
#define ArmServo1 1
#define ArmServo2 2
#define ArmServo3 3
#define ArmServo4 4
#define ArmServo5 5
#define ArmServoClaw 6
#define HBridgeEn1 7
#define HBridgeEn2 8

//default servo and motor pwm(in degrees)
#define ArmServo0DefaultAngle 45
#define ArmServo1DefaultAngle 45
#define ArmServo2DefaultAngle 45
#define ArmServo3DefaultAngle 45
#define ArmServo4DefaultAngle 45
#define ArmServo5DefaultAngle 45
#define ArmServoClawDefaultAngle 45
#define HBridgeEn1DefaultAngle 90
#define HBridgeEn2DefaultAngle 90

//angle value to increase and decrease servos
#define ArmServoAngleInc 1

//variables to store servo angles and motor speeds
float ArmServo0Angle = ArmServo0DefaultAngle;
float ArmServo1Angle = ArmServo1DefaultAngle;
float ArmServo2Angle = ArmServo2DefaultAngle;
float ArmServo3Angle = ArmServo3DefaultAngle;
float ArmServo4Angle = ArmServo4DefaultAngle;
float ArmServo5Angle = ArmServo5DefaultAngle;
float ArmServoClawAngle = ArmServoClawDefaultAngle;
float HBridgeEn1Speed = HBridgeEn1DefaultAngle;
float HBridgeEn2Speed = HBridgeEn2DefaultAngle;

//function to write the initial values to the pwm expander
void initPWMandMotor(void){
    PORTB |= (0<<PORTB.5);
    PORTB |= (0<<PORTB.2);
    PORTB |= (0<<PORTB.1);
    PORTB |= (0<<PORTB.0);
    pca9685_servo(ArmServo0, ArmServo0DefaultAngle);
    pca9685_servo(ArmServo1, ArmServo1DefaultAngle);
    pca9685_servo(ArmServo2, ArmServo2DefaultAngle);
    pca9685_servo(ArmServo3, ArmServo3DefaultAngle);
    pca9685_servo(ArmServo4, ArmServo4DefaultAngle);
    pca9685_servo(ArmServo5, ArmServo5DefaultAngle);
    pca9685_servo(ArmServoClaw, ArmServoClawDefaultAngle);
    pca9685_servo(HBridgeEn1, HBridgeEn1DefaultAngle);
    pca9685_servo(HBridgeEn2, HBridgeEn2DefaultAngle);
}

//function that sets the servo angles base on the joystick values
void setServos(){
    switch(data0){
            case 0xef://D-Pad up
                ArmServo1Angle += ArmServoAngleInc;
                pca9685_servo(ArmServo1, ArmServo1Angle);
                break;
            case 0xdf://D-Pad right
                ArmServo1Angle += ArmServoAngleInc;
                pca9685_servo(ArmServo1, ArmServo1Angle);
                break;
            case 0x7f://D-pad left
                ArmServo0Angle -= ArmServoAngleInc;
                pca9685_servo(ArmServo0, ArmServo0Angle);
                break;
            case 0xbf://D-pad down
                ArmServo0Angle -= ArmServoAngleInc;
                pca9685_servo(ArmServo0, ArmServo0Angle);
                break;
            case 0xf7://Start  
                //Toggle the motors on and off
                if(ShouldMotorsMove == 0){
                    ShouldMotorsMove = 1;
                }
                else{
                    ShouldMotorsMove = 0;
                }
                break;
        }
        switch(data1){
            case 0x7f: //square
                ArmServo3Angle -= ArmServoAngleInc;
                pca9685_servo(ArmServo3, ArmServo3Angle);
                break;
            case 0xef: //triangle
                ArmServo2Angle += ArmServoAngleInc;
                pca9685_servo(ArmServo2, ArmServo2Angle);
                break;
            case 0xdf: //circle
                ArmServo3Angle += ArmServoAngleInc;
                pca9685_servo(ArmServo3, ArmServo3Angle);
                break;
            case 0xbf: //cross
                ArmServo2Angle -= ArmServoAngleInc;
                pca9685_servo(ArmServo2, ArmServo2Angle);
                break;
            case 0xfb: //L1
                ArmServoClawAngle -= ArmServoAngleInc;
                pca9685_servo(ArmServoClaw, ArmServoClawAngle);
                break;
            case 0xf7: //R1
                ArmServoClawAngle += ArmServoAngleInc;
                pca9685_servo(ArmServoClaw, ArmServoClawAngle);
                break;
            case 0xfe: //L2
                ArmServo4Angle -= ArmServoAngleInc;
                pca9685_servo(ArmServo4, ArmServo4Angle);
                break;
            case 0xfd: //R2
                ArmServo4Angle += ArmServoAngleInc;
                pca9685_servo(ArmServo4, ArmServo4Angle);
                break;
        }
}

//function that sets the motors speed and direction 
void setMotorSpeed(int x_joy, int y_joy){
    //values from the joystick are between 0 and 0xff. we need to convert them to a value between -1 and 1
    float x = (x_joy / 127.5f) - 1;
    float y = (y_joy / 127.5f) - 1;
    float motor1 = (1 - y) * (1 - x) * 90;
    float motor2 = (1 - y) * x * 90;
    
    printf("\n\rMotors:\n\r");
    printf("motor1: %f\n'r", motor1);
    printf("motor2: %f\n\r", motor2);

    if(motor1>0){
        PORTB |= (1<<PORTB.5);
        PORTB |= (0<<PORTB.2);
        pca9685_servo(HBridgeEn1, motor1);
    }else{
        PORTB |= (0<<PORTB.5);
        PORTB |= (1<<PORTB.2);
        pca9685_servo(HBridgeEn1, -motor1);
    }
    if(motor2>0){
        PORTB |= (1<<PORTB.1);
        PORTB |= (0<<PORTB.0);
        pca9685_servo(HBridgeEn2, motor2);
    }else{
        PORTB |= (0<<PORTB.1);
        PORTB |= (1<<PORTB.0);
        pca9685_servo(HBridgeEn2, -motor2);
    }
}