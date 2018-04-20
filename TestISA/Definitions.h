//INCLUDES





//ENGINE PIN

#define A_ENABLE 2  //PWM
#define A_PHASE 48  //0 - przod, 1 - tyl DIGITAL
#define B_ENABLE 3  //PWM
#define B_PHASE 46  //0 - przod, 1 - tyl DIGITAL
#define MODE 44     //sterowanie 0 - ciężkie, 1 - uproszczone DIGITAL

#define LEFT_ENCODER 40
#define RIGHT_ENCODER 42


//speeds

#define STOP 0
#define SPEED_0 0
#define SPEED_1 100
#define SPEED_2 150
#define SPEED_3 200

//directions

#define LEFT_FRONT LOW
#define RIGHT_FRONT HIGH
#define LEFT_BACK HIGH
#define RIGHT_BACK LOW

//SERVO
#define SERVO_X 8
#define SERVO_Y 9


//DISTANCE SENSORS

#define TRIG_FRONT 8
#define ECHO_FRONT 9

#define TRIG_BACK 10
#define ECHO_BACK 11

#define TRIG_LEFT 6
#define ECHO_LEFT 7

#define TRIG_RIGHT 5
#define ECHO_RIGHT 4
