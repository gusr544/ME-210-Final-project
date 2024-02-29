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
int maxPeak = 0;
int curPeak = 0;
bool done = false;
int threshold = 62;

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
}

void turnLeft(int time) {
  digitalWrite(leftForward, 0);
  digitalWrite(rightForward, 1);

  digitalWrite(leftBack, 1);
  digitalWrite(rightBack, 0);
  delay(time);
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
  balldrop.write(180);

  // setting the speeds for the respective motors
  analogWrite(speedLeft, 230);
  analogWrite(speedRight, 235);
}

bool outOfBox = false;
bool spin = false;
int yayeet = 0;

void loop() {

  while(start_condition == false) {
    if (digitalRead(rightBumper)) {
      start_condition = true;
      //Serial.println("right");
      delay(1000);
      side = SIDEA;
    }
    if (digitalRead(leftBumper)) {
      start_condition = true;
      //Serial.println("left");
      delay(1000);
      side = SIDEB;
    }
    spin = true;
  }

  while (spin && start_condition) {
    //Serial.println("spin");
    curPeak = analogRead(beaconPin);
    Serial.println(curPeak);
    if (side == SIDEA) {
      turnRight(20);
    }
    if (side == SIDEB) {
      turnLeft(20);
    }
    if (maxPeak < curPeak && yayeet < threshold) {
      maxPeak = curPeak;
    }
    stop(50);
    yayeet ++;
    if (curPeak >= maxPeak - 10 && yayeet >= threshold) {
      spin = false;
      stop(50);
    if (side == SIDEA) {
      turnRight(150);
    }
    if (side == SIDEB) {
      turnLeft(150);
    }
      break;
    }
  }



  while(!outOfBox && yayeet > 2) {
    if (wallHits >= 3 && bigHit) {
      outOfBox = true;
      if (side == SIDEA) {
        turnRight(90);
        drive(3);
        turnRight(80);
      }
      if (side == SIDEB) {
        turnLeft(90);
        drive(3);
        turnLeft(80);
      }

      drive(22);

      if (side == SIDEA) {
        turnLeft(200);
      }
      if (side == SIDEB) {
        turnRight(200);
      }
      findingZone = true;
    }

    if (side == SIDEB) {
      ignoreRight = true;
      drive(5);
      turnLeft(20);
    }

    if (side == SIDEA) {
      ignoreLeft = true;
      drive(5);
      turnRight(20);
    }
    wallHits ++;
    if (digitalRead(centerBumper) || cBHit) {
      bigHit = true;
      wallHits = 0;
      reverse(100);
      if (side == SIDEA) {
        turnLeft(260);
      }
      if (side == SIDEB) {
        turnRight(260);
      }
    }
  }

  while(findingZone) {
    ignoreRight = false;
    ignoreLeft = false;
    forward(20);
    stop(30);
    if (digitalRead(centerBumper)) {
      contactZone = true;
      reverse(200);
      if (side == SIDEA) {
        turnRight(290);
      }
      if (side == SIDEB) {
        turnLeft(290);
      }
      findingZone = false;
      break;
    }
    if (digitalRead(rightBumper)) {
      reverse(50);
      turnRight(50);
    }
    if (digitalRead(leftBumper)) {
      reverse(50);
      turnLeft(50);
    }
  }

  while(contactZone) {
    if (side == SIDEB) {
      ignoreRight = false;
      ignoreLeft = true;
      drive(5);
      turnRight(15);
    }

    if (side == SIDEA) {
      ignoreRight = true;
      ignoreLeft = false;
      drive(5);
      turnLeft(15);
    }

    if(digitalRead(centerBumper) || cBHit) {
      while(true) {
          digitalWrite(leftForward, 0);
          digitalWrite(rightForward, 0);

          digitalWrite(leftBack, 0);
          digitalWrite(rightBack, 0);
        for (int i = 180; i >= 0; i--) {
          balldrop.write(i);
          delay(5); // Adjust delay as needed for your servo speed
        }
        for (int i = 0; i <= 180; i++) {
          balldrop.write(i);
          delay(5); // Adjust delay as needed for your servo speed
          }
  
        // Spin servo from 180 to 0 degrees

      }
    }
  }  

}
