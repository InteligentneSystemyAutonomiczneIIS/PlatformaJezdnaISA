#include "ISAMobile.h"

void SetPowerLevel(PowerSideEnum side, int level)
{
	level = constrain(level, -255, 255);
	
	if (side == PowerSideEnum::Right) {
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
	
	if (side == PowerSideEnum::Left) {
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


void setup(void)
{
	// Czujniki ultradźwiekowe
	for (int i = (int)UltraSoundSensor::__first; i <= (int)UltraSoundSensor::__last; i++)
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
	SetPowerLevel(PowerSideEnum::Left, 0);
	SetPowerLevel(PowerSideEnum::Right, 0);
	
	
	Serial.begin(9600);
	Serial.print("Test... ");
}

int measureSoundSpeed(int trigger_pin, int echo_pin)
{
	digitalWrite(trigger_pin, false);
	delayMicroseconds(2);

	digitalWrite(trigger_pin, true);
	delayMicroseconds(10);
	digitalWrite(trigger_pin, false);

	// zmierz czas przelotu fali dźwiękowej
	int duration = pulseIn(echo_pin, true, 50 * 1000);
	

	// przelicz czas na odległość (1/2 Vsound(t=20st.C))
	int distance = (int)((float)duration * 0.03438f * 0.5f);
	return distance;
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

void cmd_proximity(const char* msg, UltraSoundSensor sensor)
{
	char buffer[64];
	int d[5] = {};
	int sum = 0;
	int id = 0;
	
	while (Serial.available() == 0)
	{
		int dist = measureSoundSpeed(
			ultrasound_trigger_pin[(int)sensor],
			ultrasound_echo_pin[(int)sensor]);

		// średnia krocząca
		sum -= d[id];
		sum += d[id] = dist;
		id = (id + 1) % 5;
		dist = sum / 5;

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
		Serial.print((char)(43+2*(i&1)));
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
			Serial.println("   reset - reset");
			continue;
		}
		
		if (s == "reset") {
			Serial.println("Ok.");
			delay(1000);
			RSTC->RSTC_MR = 0xA5000F01;
			RSTC->RSTC_CR = 0xA500000D;
			while(1);
		}
		
		if (s == "proxf") {
			cmd_proximity("PRZOD", UltraSoundSensor::Front);
			continue;
		}
		
		if (s == "proxb") {
			cmd_proximity("TYL", UltraSoundSensor::Back);
			continue;
		}
		
		if (s == "proxl") {
			cmd_proximity("LEWY", UltraSoundSensor::Left);
			continue;
		}
		
		if (s == "proxr") {
			cmd_proximity("PRAWY", UltraSoundSensor::Right);
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
				SetPowerLevel(PowerSideEnum::Left, power);
			if (right)
				SetPowerLevel(PowerSideEnum::Right, power);
			
			continue;
		}

		Serial.print(" Polecenie '");
		Serial.print(s);
		Serial.print("' jest nieznane; Może 'help'?\n");
	}
}