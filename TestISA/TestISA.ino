#include "variables.h"
#include "functions.hpp"



void setup() {

    initSerial();
    initMotors();
    initServos();
    initESP826();
    initLED();
    Brake();
    delay(500);

    Serial.println("Initalization ended");

}

void loop()
{
  delay(1000);
	for (int i = 0; i < 10; i++)
	{
		Serial.print((char)(43+2*(i&1)));
		delay(200);
	}


 	Serial.println();
	Serial.println("#========================================================#");
	Serial.println("# Inteligentne Systemy Autonomiczne                      #");
	Serial.println("# Tester autek v2.0 Pawel Kapusta, Tomasz Jaworski, 2019 #");
	Serial.println("#========================================================#");
	Serial.print("  Kompilacja: ");
	Serial.print(__DATE__);
	Serial.print(", ");
	Serial.println(__TIME__);
	
	Serial.println("Polecenia powinny konczyc sie wylacznie znakiem '\\n'.");
	Serial.println("ARDUINO IDE: Zmień 'No line ending' na 'Newline' w dolnej części okna konsoli...\n");


  while(1)
	{
    // int sweepTime = 3000;
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

    if (s == "help")
		{
			Serial.println("Pomoc:");
			
			Serial.println("  mSD p   - ustaw wysterowanie silnika napędowego");
			Serial.println("	  S (strona): 'L'-lewa, 'R'-prawa, 'B'-obie");
			Serial.println("  	  D (kierunek): 'F'-do przodu, 'B'-do tyłu, 'S'-stop");
			Serial.println("   	  p (wysterowanie): poziom sterowania 0-255");
			Serial.println("  enc     - Odczyt wejść enkoderów; wcześniej uruchom silniki");
			Serial.println("  sS p   - ustaw pozycję serw");
			Serial.println("	  S (serwo): 'Y'-Yaw, 'P'-pitch, 'B'-oba");
			Serial.println("   	  p (pozycja): pozycja serwa 0-180");
			Serial.println("  reset   - reset");
			continue;
		}

    if (s == "reset") {
			Serial.println("Ok.");
			_softRestart();
      		while(1);
		}

    if (s.startsWith("m")) 
    {
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
				SetPowerLevel(EngineSelector::Left, power);
			if (right)
				SetPowerLevel(EngineSelector::Right, power);
			
			continue;
		}


    if (s.startsWith("s")) 
    {
			if (s.length() < 2) {
				Serial.println("Polecenie 's': bład w poleceniu");
				continue;
			}
			
			int direction = tolower(s[1]);
			int position = -1000;

			if (s.indexOf(" ") != -1) {
				s = s.substring(s.lastIndexOf(" ") + 1);
				char *endptr = NULL;
				position = strtol(s.c_str(), &endptr, 10);
				if (*endptr != '\0') {
					Serial.println("Polecnie 's': bład w zapisie wartości pozycji serwa");
					continue;
				}
			}
			
			if (strchr("ypb", direction) == NULL) {
				Serial.println("Polecnie 's': bład w formacie kierunku");
				continue;
			}

			if (position == -1000) {
				Serial.println("Polecnie 's': brak podanej wartości pozycji serw");
				continue;
			}
				
			// przekształcenia
			bool yaw = (side == 'y' || direction == 'b');
			bool pitch = (side == 'p' || direction == 'b');

			char msg[128];
			sprintf(msg, "Ustawienia: Y=%d, P=%d, position=%d\n", yaw, pitch, position);
			Serial.print(msg);
			if (yaw)
				moveServo(ServoSelector::Yaw, position);
			if (right)
				moveServo(ServoSelector::Pitch, position);
			continue;
		}



		Serial.print(" Polecenie '");
		Serial.print(s);
		Serial.print("' jest nieznane; Może 'help'?\n");

  }

  
}
