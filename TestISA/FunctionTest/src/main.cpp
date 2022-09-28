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
	traxxas4_tec.SetNeutral();
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
	Serial.println("# Inteligentne Systemy Autonomiczne IIS                  #");
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
				
				Serial.println("  sD p   - set the steering level");
				Serial.println("  	  D (direction): 'L'-left, 'R'-right");
				Serial.println("   	  p (level): 0-100");
				
				Serial.println("  reset   - reset");
				Serial.println("  selftest   - perform a full self-test. WARNING - WILL MOVE MOTORS FULL SPEED");
				continue;
			}

		if (s == "reset") {
				Serial.println("Ok.");
				_softRestart();
				while(1);
			}
		
		if (s == "selftest")
		{
			Serial.println("Starting self-test.");
			delay(1000);
			//TODO: 
			traxxas4_tec.SetSteering(-100);
			delay(400);
			traxxas4_tec.SetSteering(-50);
			delay(400);
			traxxas4_tec.SetSteering(0);
			delay(400);
			traxxas4_tec.SetSteering(50);
			delay(400);
			traxxas4_tec.SetSteering(100);
			delay(400);
			traxxas4_tec.SetNeutral();

			traxxas4_tec.SetSpeed(0);
			delay(400);
			traxxas4_tec.SetSpeed(20);
			delay(400);
			traxxas4_tec.SetSpeed(50);
			delay(400);
			traxxas4_tec.SetSpeed(100);
			delay(400);
			traxxas4_tec.SetSpeed(0);
			delay(400);
			traxxas4_tec.SetSpeed(-20);
			delay(400);
			traxxas4_tec.SetSpeed(-50);
			delay(400);
			traxxas4_tec.SetSpeed(-100);
			delay(400);
			traxxas4_tec.SetNeutral();

		}

		if (s.startsWith("m")) 
		{
				if (s.length() < 2) {
					Serial.println("Command 'm': command syntax error");
					continue;
				}
				
				int direction = tolower(s[1]);
				int power = 0;
				if (s.indexOf(" ") != -1) {
					s = s.substring(s.lastIndexOf(" ") + 1);
					char *endptr = NULL;
					power = strtol(s.c_str(), &endptr, 10);
					if (*endptr != '\0') {
						Serial.println("Command 'm': error in power level syntax");
						continue;
					}
				}
				power = constrain(power, 0, 100);
				
				if (strchr("frs", direction) == NULL) {
					Serial.println("Command 'm': error in direction syntax");
					continue;
				}
				
				if (direction != 's' && power == 0) {
					Serial.println("Command 'm': power level not set properly");
					continue;
				}
					

				if (direction == 'r')
				{
					power *= -1;
				}
	

				char msg[128];
				sprintf(msg, "Motor settings: power=%d\n", power);
				Serial.print(msg);
				traxxas4_tec.SetSpeed(power);
				
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
				
				if (level == -1000) {
					Serial.println("Command 's': level not set properly");
					continue;
				}
				level = constrain(level, 0, 100);

				if (strchr("lr", direction) == NULL) {
					Serial.println("Command 's': direction syntax error");
					continue;
				}

				if (direction == 'l')
				{
					level*=-1;
				}

				char msg[128];
				sprintf(msg, "Steering servo setting: position/level=%d\n", level);
				Serial.print(msg);
				traxxas4_tec.SetSteering(level);
				continue;
			}

			Serial.print(" Command '");
			Serial.print(s);
			Serial.print("' is unknown; Maybe try 'help'?\n");

	}

}