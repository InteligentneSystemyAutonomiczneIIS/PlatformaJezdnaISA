DistanceSensor(){
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(TRIG_BACK, OUTPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(TRIG_RIGHT, OUTPUT);

  pinMode(ECHO_FRONT, INPUT);
  pinMode(ECHO_BACK, INPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(ECHO_RIGHT, INPUT);
}

long STime, SDistance;

long getDistance(int sensor){
  if(sensor == 1){  //front
    digitalWrite(TRIG_FRONT, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_FRONT, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_FRONT, LOW);

    STime = pulseIn(ECHO_FRONT, HIGH);    
  }

  if(sensor == 2){  //back
    digitalWrite(TRIG_BACK, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_BACK, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_BACK, LOW);

    STime = pulseIn(ECHO_BACK, HIGH);
  }

  if(sensor == 3){  //left
    digitalWrite(TRIG_LEFT, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_LEFT, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_LEFT, LOW);

    STime = pulseIn(ECHO_LEFT, HIGH);
  }

  if(sensor == 4){  //right
    digitalWrite(TRIG_RIGHT, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_RIGHT, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_RIGHT, LOW);

    STime = pulseIn(ECHO_RIGHT, HIGH);
  }
  
  SDistance = STime / 58;
  return SDistance;
}

