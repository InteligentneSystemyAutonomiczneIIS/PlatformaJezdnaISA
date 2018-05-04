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
	int duration = pulseIn(echo, true, 50 * 1000);
	

	// przelicz czas na odległość (1/2 Vsound(t=20st.C))
	int distance = (int)((float)duration * 0.03438f * 0.5f);
	return distance;
}

void SetPowerLevel(int side, int level)
{
	level = constrain(level, -255, 255);
	
	if (side == Side_Right) {
		if (level > 0) {
			// do przodu
			digitalWrite(A_PHASE, 1);
			analogWrite(A_ENABLE, level);
		} else if (level < 0) {
			// do tyłu
			digitalWrite(A_PHASE, 0);
			analogWrite(A_ENABLE, -level);
		} else {
			// stop
			digitalWrite(A_PHASE, 0);
			analogWrite(A_ENABLE, 0);
		}
	}
	
	if (side == Side_Left) {
		if (level > 0) {
			// do przodu
			digitalWrite(B_PHASE, 1);
			analogWrite(B_ENABLE, level);
		} else if (level < 0) {
			// do tyłu
			digitalWrite(B_PHASE, 0);
			analogWrite(B_ENABLE, -level);
		} else {
			// stop
			digitalWrite(B_PHASE, 0);
			analogWrite(B_ENABLE, 0);
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

void cmd_proximity(const char* msg, int us_sensor)
{
	char buffer[64];
	while (Serial.available() == 0)
	{
		int dist = measure(
			ultrasound_trigger_pin[us_sensor],
			ultrasound_echo_pin[us_sensor]);
			
		sprintf(buffer, "\n%s: %0dcm", msg, dist);
		Serial.print(buffer);
	}
	
	while (Serial.available())
		Serial.read();	
}

void loop(void)
{
	delay(1000);
	for (int i = 0; i < 10; i++)
	{
		Serial.print("+");
		delay(200);
	}
	Serial.println();
	Serial.println("=======================================================");
	Serial.println("# Programowanie Systemow Autonomicznych               #");
	Serial.println("# Tester autek v1.0 Tomasz Jaworski, 2018             #");
	Serial.println("=======================================================");
	Serial.println("Polecenia powinny konczyc sie wylacznie znakiem '\\n'.");
	Serial.println("ARDUINO IDE: Zmień 'No line ending' na 'Newline' w dolnej części okna konsoli...\n");
	
	while(1)
	{
		Serial.print("> ");

		String s = "";
		while(true)
		{
			while(Serial.available() == 0);
			int ch = Serial.read();
			if (ch == '\n')
				break;
			s += (char)ch;
		}
		
		s.trim();
		s.toLowerCase();
		Serial.println(s);
		
		//
		
		if (s == "help")
		{
			Serial.println("Pomoc:");
			Serial.println("   proxf - odczytuj czujnik odleglosc (PRZEDNI)");
			Serial.println("   proxb - odczytuj czujnik odleglosc (TYLNY)");
			Serial.println("   proxl - odczytuj czujnik odleglosc (LEWY)");
			Serial.println("   proxr - odczytuj czujnik odleglosc (PRAWY)");
			
			Serial.println("   mSD p - ustaw wysterowanie silnika napędowego");
			Serial.println("   		S (strona): 'L'-lewa, 'R'-prawa, 'B'-obie");
			Serial.println("   		D (kierunek): 'F'-do przodu, 'B'-do tyłu, 'S'-stop");
			Serial.println("   		n (wysterowanie): poziom sterowania 0-255");
			continue;
		}
		
		if (s == "proxf") {
			cmd_proximity("PRZOD", US_FRONT);
			continue;
		}
		
		if (s == "proxb") {
			cmd_proximity("TYL", US_BACK);
			continue;
		}
		
		if (s == "proxl") {
			cmd_proximity("LEWY", US_LEFT);
			continue;
		}
		
		if (s == "proxr") {
			cmd_proximity("PRAWY", US_RIGHT);
			continue;
		}

		if (s.startsWith("m")) {
			if (s.length() < 3) {
				Serial.println("Polecenie 'm': bład w poleceniu");
				continue;
			}
			
			int side = tolower(s[1]);
			int direction = tolower(s[2]);
			int power = -1;
			if (s.indexOf(" ") != -1) {
				s = s.substring(s.lastIndexOf(" ") + 1);
				char *endptr = NULL;
				power = strtol(s.c_str(), &endptr, 10);
				if (*endptr != '\0') {
					Serial.println("Polecnie 'm': bład w zapisie wartości wystarowania");
					continue;
				}
			}
			
			if (strchr("lrb", side) == NULL) {
				Serial.println("Polecnie 'm': bład w formacie strony");
				continue;
			}
				
			if (strchr("fbs", direction) == NULL) {
				Serial.println("Polecnie 'm': bład w formacie kierunku");
				continue;
			}
			
			if (direction != 's' && power == -1) {
				Serial.println("Polecnie 'm': brak podanej wartości wysterowania");
				continue;
			}
				
			// przekształcenia
			bool left = side == 'l' || side == 'b';
			bool right = side == 'r' || side == 'b';
			power = direction == 's' ? 0 : power;
			power = direction == 'b' ? -power : power;

			char msg[128];
			sprintf(msg, "Ustawienia: L=%d, R=%d, power=%d\n", left, right, power);
			Serial.print(msg);
			if (left)
				SetPowerLevel(Side_Left, power);
			if (right)
				SetPowerLevel(Side_Right, power);
			
			continue;
		}

		Serial.print(" Polecenie '");
		Serial.print(s);
		Serial.print("' jest nieznane; Może 'help'?\n");
	}
}