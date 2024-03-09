#define TURN_SPEED 100
#define FULL_SPEED 500
#define HALF_SPEED 250
#define MOTOR_OFFSET 5
#define LINE_THRESHOLD 200

#include <Servo.h>

// Right Motor Pins
const int speedRight = 11;
const int rightForward = 13;
const int rightBack = 12;

// Left Motor Pins
const int speedLeft= 6;
const int leftForward = 7;
const int leftBack = 8;

// Bumper Pins
const int leftBumper = 3;
const int centerBumper = 4;
const int rightBumper = 5;

// Servo
const int servoPin = A0;

// Line Sensors
const int leftPin = A2;
const int leftCenterPin = A3;
const int rightCenterPin = A4;
const int rightPin = A5;

// State Definitions
typedef enum {
  MOVING_FORWARD, ALIGNING_LEFT_WALL, ALIGNING_RIGHT_WALL, 
  TURNING_LEFT, TURNING_RIGHT, SPINNING, LINE_FOLLOW_FORWARD, 
  WALL_FOLLOW_FORWARD, BALL_DROP, STOP, MOVING_BACKWARD
} States_t;

States_t state;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // begin serial communication
  Serial.println("Hello, World!");

  // Set Motor Pins to output
  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftBack, OUTPUT);

  // Set Bumper Pins to input pullup
  pinMode(leftBumper, INPUT_PULLUP);
  pinMode(rightBumper, INPUT_PULLUP);
  pinMode(centerBumper, INPUT_PULLUP);

  // Set motor speed
  analogWrite(speedRight, 100);
  analogWrite(speedLeft, 100);
}

void loop() {
  // put your main code here, to run repeatedly:
  lineFollowTest();
  // forward(FULL_SPEED);
}

// LINE FOLLOWING
void lineFollowForward(int speed) {
  while (state = LINE_FOLLOW_FORWARD) {
    // All four pins aren't on line, move forward
    if (!readSensor(leftPin) && !readSensor(leftCenterPin) && 
    !readSensor(rightPin) && !readSensor(rightCenterPin)) {
      forward(speed);
    }
    
    // Right Center is on line, turn right
    else if (readSensor(rightCenterPin)) {
      turnRight(); // Motor Control turning right
    }
    
    // Left Center is on line, turn left
    else if (readSensor(leftCenterPin)) {
      turnLeft(); // Motor Control turning right
    }

    // All sensors are on line
    else if (readSensor(leftPin) && readSensor(leftCenterPin) && 
    readSensor(rightPin) && readSensor(rightCenterPin)) {
      break;
    }
  }
}

void lineFollowTest() {
  state = LINE_FOLLOW_FORWARD;
  lineFollowForward(100);
}

// Read line sensor analog value, return true for line and false otherwise
bool readSensor(int pin) {
  if (analogRead(pin) >= LINE_THRESHOLD) {
    return true; // on line
  } else {
    return false; // not on line
  }
}

// MOTOR CONTROL
void forward(int speed) {
  rightMotorSpeed(speed);
  leftMotorSpeed(speed - MOTOR_OFFSET);
}

void backward(int speed) {
  rightMotorSpeed(-1 * speed);
  leftMotorSpeed(-1 * (speed - MOTOR_OFFSET));
}

void turnRight() {
  rightMotorSpeed(-1 * TURN_SPEED);
  leftMotorSpeed(TURN_SPEED - MOTOR_OFFSET);
}

void turnLeft() {
  rightMotorSpeed(TURN_SPEED);
  leftMotorSpeed(-1 * (TURN_SPEED - MOTOR_OFFSET));
}

void stop() {
  rightMotorSpeed(0);
  leftMotorSpeed(0);
  return;
}

void rightMotorSpeed(int speed) {
  // Set motor speed
  analogWrite(speedRight, abs(speed));

  if (speed == 0) {
    // Set right motor pins low
    digitalWrite(rightForward, LOW);
    digitalWrite(rightBack, LOW);
  }

  else if (speed > 0) {
    // Set right forward high, right back low
    digitalWrite(rightForward, HIGH);
    digitalWrite(rightBack, LOW);
  }

  else { // speed is negative
    digitalWrite(rightForward, LOW);
    digitalWrite(rightBack, HIGH);
  }

  return;
}

void leftMotorSpeed(int speed) {
  // Set motor speed
  analogWrite(speedLeft, abs(speed));

  if (speed == 0) {
    // Set right motor pins low
    digitalWrite(leftForward, LOW);
    digitalWrite(leftBack, LOW);
  }

  else if (speed > 0) {
    // Set right forward high, right back low
    digitalWrite(leftForward, HIGH);
    digitalWrite(leftBack, LOW);
  }

  else { // speed is negative
    digitalWrite(leftForward, LOW);
    digitalWrite(leftBack, HIGH);
  }

  return;
}