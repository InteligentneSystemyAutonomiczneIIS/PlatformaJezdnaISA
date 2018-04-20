#if !defined(_ISA_MOBILE_H_)
#define _ISA_MOBILE_H_

/*
 * Czujniki odległości
 */


#define US_FRONT				0
#define US_FRONT_TRIGGER_PIN	8
#define US_FRONT_ECHO_PIN		9

#define US_BACK					1
#define US_BACK_TRIGGER_PIN		10
#define US_BACK_ECHO_PIN		11

#define US_LEFT					2
#define US_LEFT_TRIGGER_PIN		6
#define US_LEFT_ECHO_PIN		7

#define US_RIGHT				3
#define US_RIGHT_TRIGGER_PIN	5
#define US_RIGHT_ECHO_PIN		4
 
 
int ultrasound_trigger_pin[] = {
	[US_FRONT] = US_FRONT_TRIGGER_PIN,
	[US_BACK] = US_BACK_TRIGGER_PIN,
	[US_LEFT] = US_LEFT_TRIGGER_PIN,
	[US_RIGHT] = US_RIGHT_TRIGGER_PIN,
};

 
int ultrasound_echo_pin[] = {
	[US_FRONT]	= US_FRONT_ECHO_PIN,
	[US_BACK]	= US_BACK_ECHO_PIN,
	[US_LEFT]	= US_LEFT_ECHO_PIN,
	[US_RIGHT]	= US_RIGHT_ECHO_PIN,
};

 
#endif // _ISA_MOBILE_H_
