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
