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
mapSide side = UNKNOWN;
int wallHits = 0;

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
  turnLeft(70);
}

void respLeftBumper() {
  reverse(20);
  turnRight(70);
}

void drive(int cycles) {
  for (int i = 0; i <= cycles; i ++) {
    forward(30);
    if (digitalRead(leftBumper)) {
      respLeftBumper();
      wallHits = 0;
    }
    if (digitalRead(rightBumper)) {
      respRightBumper();
      wallHits = 0;
    }
    if (digitalRead(centerBumper)) {
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

  // setting the speeds for the respective motors
  analogWrite(speedLeft, 230);
  analogWrite(speedRight, 235);
}

bool outOfBox = false;


void loop() {

  while(start_condition == false) {
    if (digitalRead(rightBumper)) {
      start_condition = true;
      delay(1000);
      side = SIDEA;
    }
    if (digitalRead(leftBumper)) {
      start_condition = true;
      delay(1000);
      side = SIDEB;
    }
  }

  while(!outOfBox) {
    if (wallHits >= 4 && bigHit) {
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

      drive(20);

      if (side == SIDEA) {
        turnLeft(280);
      }
      if (side == SIDEB) {
        turnRight(280);
      }
      findingZone = true;
    }

    if (side == SIDEB) {
      drive(5);
      turnLeft(20);
    }

    if (side == SIDEA) {
      drive(5);
      turnRight(20);
    }
    wallHits ++;
    if (digitalRead(centerBumper)) {
      bigHit = true;
      wallHits = 0;
      reverse(100);
      if (side == SIDEA) {
        turnLeft(400);
      }
      if (side == SIDEB) {
        turnRight(400);
      }
    }
  }

  while(findingZone) {
    if (digitalRead(centerBumper)) {
      contactZone = true;
      reverse(200);
      if (side == SIDEA) {
        turnRight(400);
      }
      if (side == SIDEB) {
        turnLeft(400);
      }
      findingZone = false;
    }
    forward(35);
    stop(50);
    if (digitalRead(rightBumper)) {
      reverse(300);
      turnRight(200);
    }
    if (digitalRead(leftBumper)) {
      reverse(300);
      turnLeft(200);
    }
  }

  while(contactZone) {
    if (side == SIDEB) {
      drive(5);
      turnRight(50);
    }

    if (side == SIDEA) {
      drive(5);
      turnLeft(50);
    }

    if(digitalRead(centerBumper)) {
      stop(15000);
    }
  }  

}
