#include "ISAMobile.h"

enum Side
{
	Side_Left,
	Side_Right
};



void SetPowerLevel(int side, int level);

void setup(void)
{
	// Czujniki ultradźwiekowe
	for (int i = 0; i < 4; i++)
	{
		pinMode(ultrasound_trigger_pin[i], OUTPUT);
		pinMode(ultrasound_echo_pin[i], INPUT);
		
		digitalWrite(ultrasound_trigger_pin[i], 0);
	}
	
	// Silniki
	pinMode(A_PHASE, OUTPUT);
	pinMode(A_ENABLE, OUTPUT);
	pinMode(B_PHASE, OUTPUT);
	pinMode(B_ENABLE, OUTPUT);
	pinMode(MODE, OUTPUT);

	digitalWrite(MODE, true);
	SetPowerLevel(Side_Left, 0);
	SetPowerLevel(Side_Right, 0);
	
	
	Serial.begin(9600);
	Serial.println("Test");
}

int measure(int trigger, int echo)
{
	digitalWrite(trigger, false);
	delayMicroseconds(2);

	digitalWrite(trigger, true);
	delayMicroseconds(10);
	digitalWrite(trigger, false);

	// zmierz czas przelotu fali dźwiękowej
	int duration = pulseIn(echo, true);

	// przelicz czas na odległość (1/2 Vsound(t=20st.C))
	int distance = (int)((float)duration * 0.03438f * 0.5f);
	return distance;
}
/*
void loop(void)
{
	
	int dfront = measure(US_FRONT_TRIGGER_PIN, US_FRONT_ECHO_PIN);
	int dback = measure(US_BACK_TRIGGER_PIN, US_BACK_ECHO_PIN);
	int dleft = measure(US_LEFT_TRIGGER_PIN, US_LEFT_ECHO_PIN);
	int dright = measure(US_RIGHT_TRIGGER_PIN, US_RIGHT_ECHO_PIN);
	
	char buffer[64];
	sprintf(buffer, "Distance: Front=%03d Back=%03d Left=%03d Right=%03d",
		dfront, dback, dleft, dright);
	Serial.println(buffer);
}
*/

void SetPowerLevel(int side, int level)
{
	if (side == Side_Left)
	{
		if (level > 0)
		{
			// do przodu
			digitalWrite(A_PHASE, 1);
			analogWrite(B_ENABLE, level);
		} else if (level < 0)
		{
			// do tyłu
			digitalWrite(A_PHASE, 0);
			analogWrite(B_ENABLE, 255 - -level);
		} else
		{
			// stop
			digitalWrite(A_PHASE, 0);
			analogWrite(B_ENABLE, 255);
		}
	}
	
	if (side == Side_Right)
	{
		if (level > 0)
		{
			// do przodu
			digitalWrite(B_PHASE, 0);
			analogWrite(A_ENABLE, level);
		} else if (level < 0)
		{
			// do tyłu
			digitalWrite(B_PHASE, 1);
			analogWrite(A_ENABLE, 255 - -level);
		} else
		{
			// stop
			digitalWrite(B_PHASE, 0);
			analogWrite(A_ENABLE, 0);
		}
	}	
}

/*
void loop(void)
{
	delay(1000);


	SetPowerLevel(Side_Left, 100);
	delay(2000);

	SetPowerLevel(Side_Left, 200);
	delay(2000);

	SetPowerLevel(Side_Left, 255);
	delay(2000);


	SetPowerLevel(Side_Left, 0);
	SetPowerLevel(Side_Right, 0);

	SetPowerLevel(Side_Left, -100);
	delay(2000);

	SetPowerLevel(Side_Left, -200);
	delay(2000);

	SetPowerLevel(Side_Left, -255);
	delay(2000);

	SetPowerLevel(Side_Left, 0);
	SetPowerLevel(Side_Right, 0);
	delay(4000);
	
	
	////	
	
	delay(1000);

	SetPowerLevel(Side_Right, 100);
	delay(2000);

	SetPowerLevel(Side_Right, 200);
	delay(2000);

	SetPowerLevel(Side_Right, 255);
	delay(2000);


	SetPowerLevel(Side_Right, 0);
	SetPowerLevel(Side_Right, 0);

	SetPowerLevel(Side_Right, -100);
	delay(2000);

	SetPowerLevel(Side_Right, -200);
	delay(2000);

	SetPowerLevel(Side_Right, -255);
	delay(2000);

	SetPowerLevel(Side_Right, 0);
	SetPowerLevel(Side_Right, 0);
	delay(4000);
}
*/

void loop(void)
{
	delay(1000);
	for (int i = 0; i < 10)
	{
		Serial.print("+");
		delay(200);
	}
	
	Serial.println("=======================================================");
	Serial.println("# Programowanie Systemow Autonomicznych               #");
	Serial.println("# Tester autek v1.0 Tomasz Jaworski, 2018             #");
	Serial.println("=======================================================");
	
	while(1)
	{
		Serial.print("> ");
		
