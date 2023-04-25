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
#define HBridgeEn1DefaultAngle 45
#define HBridgeEn2DefaultAngle 45

//angle value to increase and decrease servos
#define ArmServo0AngleInc 5

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
//function that sets the motors speed and direction 
void setMotorSpeed(int8_t motor1, int8_t motor2){
    if(motor1>0){
        PORTB |= (1<<PORTB.5)
        PORTB |= (0<<PORTB.2)
        pca9685_servo(HBridgeEn1, motor1);
    }else{
        PORTB |= (0<<PORTB.5)
        PORTB |= (1<<PORTB.2)
        pca9685_servo(HBridgeEn1, -motor1);
    }
    if(motor2>0){
        PORTB |= (1<<PORTB.1)
        PORTB |= (0<<PORTB.0)
        pca9685_servo(HBridgeEn2, motor2);
    }else{
        PORTB |= (0<<PORTB.1)
        PORTB |= (1<<PORTB.0)
        pca9685_servo(HBridgeEn2, -motor2);
    }
}