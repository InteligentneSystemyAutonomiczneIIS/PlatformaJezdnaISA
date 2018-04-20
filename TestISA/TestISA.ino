#include "ISAMobile.h"

void setup(void)
{
	// Czujniki ultradźwiekowe
	for (int i = 0; i < 4; i++)
	{
		pinMode(ultrasound_trigger_pin[i], OUTPUT);
		pinMode(ultrasound_echo_pin[i], INPUT);
		
		digitalWrite(ultrasound_trigger_pin[i], 0);
	}
	
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

