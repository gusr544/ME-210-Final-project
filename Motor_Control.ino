const int speedRight = 11;
const int rightForward = 13;
const int rightBack = 12;
const int speedLeft= 6;
const int leftForward = 7;
const int leftBack = 8;
const int leftBumper = 3;
const int centerBumper = 4;
const int rightBumper = 5;

void reverse(int time);
void respLeftBumper(void);
void respRightBumper(void);
void respCenterBumper(void);
void turnLeft(int time);
void turnRight(int time);

void reverse(int time) {
  digitalWrite(leftForward, 0);
  digitalWrite(rightForward, 0);

  digitalWrite(leftBack, 1);
  digitalWrite(rightBack, 1);
  delay(time);
}

void turnRight(int time) {
  digitalWrite(leftForward, 1);
  digitalWrite(rightForward, 0);

  digitalWrite(leftBack, 0);
  digitalWrite(rightBack, 1);
  delay(time);
}

void turnLeft(int time) {
  digitalWrite(leftForward, 0);
  digitalWrite(rightForward, 1);

  digitalWrite(leftBack, 1);
  digitalWrite(rightBack, 0);
  delay(time);
}

void respCenterBumper(){
  reverse(3000);
}

void respRightBumper(){
  reverse(3000);
  turnRight(500);
}

void respLeftBumper() {
  reverse(3000);
  turnLeft(500);
}

void setup() {
  
  Serial.begin(9600);  // Set the baud rate to match the Serial Monitor
  Serial.println("hello world!");


  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftBack, OUTPUT);
  pinMode(leftBumper, INPUT);
  pinMode(rightBumper, INPUT);
  pinMode(centerBumper, INPUT);
}



void loop() {
  analogWrite(speedLeft, 177);
  analogWrite(speedRight, 150);

  digitalWrite(leftBack, 0);
  digitalWrite(rightBack, 0);

  digitalWrite(leftForward, 1);
  digitalWrite(rightForward, 1);

  //delay(1000);

  
  if (digitalRead(leftBumper)) {
    respLeftBumper();
  }
  else if(digitalRead(rightBumper)) {
    respRightBumper();
  }
  else if (digitalRead(centerBumper)) {
    respCenterBumper();
  }

/*
  digitalWrite(leftBack, 1);
  digitalWrite(rightBack, 1);

  delay(400);

  digitalWrite(leftBack, 0);
  digitalWrite(rightBack, 0);

  delay(1500);
*/
}
