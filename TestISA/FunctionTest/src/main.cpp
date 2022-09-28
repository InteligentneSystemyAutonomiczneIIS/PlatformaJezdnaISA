#include <Arduino.h>
#include <functions.hpp>
#include <variables.h>
#include <Chasis.hpp>

Chasis traxxas4_tec;



void setup() {
  	// put your setup code here, to run once:
	
	// Initialize Serial communication
    initSerial();


	//initialize LED
    initLED();

	// Initialize Chassis
	traxxas4_tec.Initialize();
	//make sure that motors and servos are in neutral position
	traxxas4_tec.SetSteering(0.0f);
	traxxas4_tec.SetSpeed(0.0f);
    delay(500);

    Serial.println("Initalization ended");

	delay(1000);
	for (int i = 0; i < 10; i++)
	{
		Serial.print((char)(43+2*(i&1)));
		delay(200);
	}


 	Serial.println();
	Serial.println("#========================================================#");
	Serial.println("# Inteligentne Systemy Autonomiczne                      #");
	Serial.println("# Tester platform v3.0 Pawel Kapusta                     #");
	Serial.println("# original code by Tomasz Jaworski                       #");
	Serial.println("#========================================================#");
	Serial.print("  Kompilacja: ");
	Serial.print(__DATE__);
	Serial.print(", ");
	Serial.println(__TIME__);
	
	Serial.println("Commands must end with character: '\\n'.");
	Serial.println("ARDUINO IDE: Change 'No line ending' to 'Newline' in the lower portion of console window...\n");

	delay(2000);
}

void loop() {
  	// put your main code here, to run repeatedly:
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

		if (s == "help")
			{
				Serial.println("Help:");
				
				Serial.println("  mD p   - set the motor speed and direction");
				Serial.println("  	  D (direction): 'F'-forwards, 'R'-reverse, 'S'-stop");
				Serial.println("   	  p (level/speed): 0-100");
				// Serial.println("  enc     - Odczyt wejść enkoderów; wcześniej uruchom silniki");
				
				Serial.println("  sD p   - set the steering level");
				Serial.println("  	  D (direction): 'L'-left, 'R'-right");
				Serial.println("   	  p (level): 0-100");
				
				// Serial.println("  sS p   - ustaw pozycję serw");
				// Serial.println("	  S (serwo): 'Y'-Yaw, 'P'-pitch, 'B'-oba");
				// Serial.println("   	  p (pozycja): pozycja serwa 0-180");
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
				if (s.length() < 2) {
					Serial.println("Command 'm': command syntax error");
					continue;
				}
				
				int direction = tolower(s[1]);
				int power = -1;
				if (s.indexOf(" ") != -1) {
					s = s.substring(s.lastIndexOf(" ") + 1);
					char *endptr = NULL;
					power = strtol(s.c_str(), &endptr, 10);
					if (*endptr != '\0') {
						Serial.println("Command 'm': error in power level syntax");
						continue;
					}
				}
				
				if (strchr("frs", direction) == NULL) {
					Serial.println("Command 'm': error in direction syntax");
					continue;
				}
				
				if (direction != 's' && power == -1) {
					Serial.println("Command 'm': no power level set");
					continue;
				}
					
				// int to float for chassis

				power = direction == 's' ? 0 : power;
				power = direction == 'r' ? -power : power;
				power = constrain(power, -100, 100);
				float powerConverted = float((( (float)power - (-1.0f)) * (100 - (-100))) / (1.0f - (-1.0f)));

				char msg[128];
				sprintf(msg, "Motor settings: power=%d\n", power);
				Serial.print(msg);
				traxxas4_tec.SetSpeed(powerConverted);
				
				continue;
			}


		if (s.startsWith("s")) 
		{
				if (s.length() < 2) {
					Serial.println("Command 's': command syntax error");
					continue;
				}
				
				int direction = tolower(s[1]);
				int level = -1000;

				if (s.indexOf(" ") != -1) {
					s = s.substring(s.lastIndexOf(" ") + 1);
					char *endptr = NULL;
					level = strtol(s.c_str(), &endptr, 10);
					if (*endptr != '\0') {
						Serial.println("Command 's': servo position syntax error");
						continue;
					}
				}
				
				if (strchr("", direction) == NULL) {
					Serial.println("Command 's': direction syntax error");
					continue;
				}

				if (level == -1000) {
					Serial.println("Command 's': level not set properly");
					continue;
				}
					
				// przekształcenia

				level = constrain(level, 0, 100);
				if (direction == 'l')
				{
					level*=-1;
				}

				float levelConverted = float((( (float)level - (-1.0f)) * (100 - (-100))) / (1.0f - (-1.0f)));

				char msg[128];
				sprintf(msg, "Steering servo setting: position/level=%d\n", level);
				Serial.print(msg);
				traxxas4_tec.SetSteering(levelConverted);
				continue;
			}



			Serial.print(" Command '");
			Serial.print(s);
			Serial.print("' is unknown; Maybe try 'help'?\n");

	}

}