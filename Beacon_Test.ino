#include <Servo.h>
const int speedRight = 11;
const int rightForward = 13;
const int rightBack = 12;
const int speedLeft= 6;
const int leftForward = 8;
const int leftBack = 7;
const int leftBumper = 3;
const int centerBumper = 4;
const int rightBumper = 5;
const int servoPin = A1;
const int beaconPin = A4;

void reverse(int time);
void forward(int time);
void stop(int time);
void respLeftBumper(void);
void respRightBumper(void);
void respCenterBumper(void);
void turnLeft(int time);
void turnRight(int time);
void drive(int cycles);

enum mapSide {
  SIDEA,
  SIDEB,
  UNKNOWN
};

bool start_condition = false;
bool findingZone = false;
bool contactZone = false;
bool approachShooter = false;
bool bigHit = false;
bool ballDrop = false;
Servo balldrop;
mapSide side = UNKNOWN;
int wallHits = 0;
bool cBHit = false;
bool ignoreLeft = false;
bool ignoreRight = false;

void stop(int time) {
  digitalWrite(leftForward, 0);
  digitalWrite(rightForward, 0);

  digitalWrite(leftBack, 0);
  digitalWrite(rightBack, 0);
  delay(time);
}

void forward(int time) {
  digitalWrite(leftForward, 1);
  digitalWrite(rightForward, 1);

  digitalWrite(leftBack, 0);
  digitalWrite(rightBack, 0);
  delay(time);
}

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
  digitalWrite(leftForward, 0);
  digitalWrite(rightForward, 0);

  digitalWrite(leftBack, 0);
  digitalWrite(rightBack, 0);
}

void turnLeft(int time) {
  digitalWrite(leftForward, 0);
  digitalWrite(rightForward, 1);

  digitalWrite(leftBack, 1);
  digitalWrite(rightBack, 0);
  delay(time);
  digitalWrite(leftForward, 0);
  digitalWrite(rightForward, 0);

  digitalWrite(leftBack, 0);
  digitalWrite(rightBack, 0);
}

void respCenterBumper(){
  reverse(10);
}

void respRightBumper(){
  reverse(20);
  turnLeft(30);
}

void respLeftBumper() {
  reverse(20);
  turnRight(30);
}

void drive(int cycles) {
  for (int i = 0; i <= cycles; i ++) {
    forward(30);
    if (digitalRead(centerBumper)) {
      cBHit = true;
      break;
    }
    if (digitalRead(leftBumper) && !ignoreLeft) {
      cBHit = false;
      respLeftBumper();
      wallHits = 0;
    }
    if (digitalRead(rightBumper) && !ignoreRight) {
      cBHit = false;
      respRightBumper();
      wallHits = 0;
    }
    stop(50);
  }
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
  balldrop.attach(servoPin);
  balldrop.write(80);

  // setting the speeds for the respective motors
  analogWrite(speedLeft, 230);
  analogWrite(speedRight, 235);
}

bool outOfBox = false;
int maxPeak = 0;
int curPeak = 0;
bool done = false;
int i = 0;
int threshold = 62;

void loop() {
  curPeak = analogRead(beaconPin);
  Serial.println(curPeak);
  turnLeft(20);
  if (maxPeak < curPeak && i < threshold) {
    maxPeak = curPeak;
  }
  stop(50);
  i ++;
  if (curPeak >= maxPeak - 10 && i >= threshold) {
    turnRight(300);
    stop(500000);
  }


}
