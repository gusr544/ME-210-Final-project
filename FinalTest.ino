#include <Servo.h>
/* This file is used as our MVP for the check off. It is imperfect, but please
  do not mae=ke changes directly to this document, per request of the teaching team. They wanted us to 
  preserve this for the competition. Feel free to copy it and make changes though, the code is far from perfect.
*/

// Pinouts based off the arduino
const int speedRight = 11;
const int rightForward = 13;
const int rightBack = 12;
const int speedLeft= 6;
const int leftForward = 7;
const int leftBack = 8;
const int leftBumper = 5;
const int centerBumper = 4;
const int rightBumper = 3;
const int servoPin = A0;
const int beaconPin = A1;

void reverse(int time);
void forward(int time);
void stop(int time);
void respLeftBumper(void);
void respRightBumper(void);
void respCenterBumper(void);
void turnLeft(int time);
void turnRight(int time);
void drive(int cycles);

// enum to say which side of the map billy is on. SIDEA is the side of the map closer to the garage door. Its bumper is on the right side of the 
// robot. SIDEB is the side more into the building, and the left bumper is used to comminicate that that is that side.
enum mapSide {
  SIDEA,
  SIDEB,
  UNKNOWN
};


// Globals to control states and other information

// start condition checks when the initial bumper has been hit to show which side Billy is on
bool start_condition = false;
// bool to control when billy is scanning for the contact zone
bool findingZone = false;
// bool to show that billy has now found the contact zone
bool contactZone = false;
// bool to control when the center bumper is hit during the first wall following segement
bool bigHit = false;
// bool to control when the MVP is complete and it is time to drop the balls
bool ballDrop = false;
// global servo that controls our servo
Servo balldrop;
// global mapSide that controls which side of the map billy is on
mapSide side = UNKNOWN;
// global for wall follwing, used to determine the amount of time that has passed since billy last hit a wall
int wallHits = 0;
// bool to control for whether or not the center bumper has been hit in some wall following programs
bool cBHit = false;
// these bools tell billy to ignore either the left or right bumper based on what side of the map hes on
bool ignoreLeft = false;
bool ignoreRight = false;
// these integers are used for beacon detection and finding the max peak based on one full rotation in the start box
int maxPeak = 0;
int curPeak = 0;
// used in beacon detection to make sure billy has compeleted an intire spin
int spin_threshold = 70;
// used to tell if billy has passed the two initial walls and made it to the main map
bool outOfBox = false;
// bool to control when billy is in the spinning/ scanning phase
bool spin = false;
// counts the number of turns the robot has completed while spinning, used in tandem with spin_threshold
int numTurns = 0;


// these next functions are a set of simple macros that set the input pins on the motors to a certin state for a given period of time.
// they do what their function says they do
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

// these functions are written to respond to input based off which bumper is hit. it controls the robot using the prior commands
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

// this function is our way of driving while recieving input at the same time. It loops through on a given number of cycles, driving
// a little bit, then checking the bumpers, then driving some more. It does this fast enough to not miss input from sensors.
void drive(int cycles) {
  for (int i = 0; i <= cycles; i ++) {
    cBHit = false;
    forward(30);
    if (!digitalRead(centerBumper)) {
      cBHit = true;
      break;
    }
    if (!digitalRead(leftBumper) && !ignoreLeft) {
      cBHit = false;
      respLeftBumper();
      wallHits = 0;
    }
    if (!digitalRead(rightBumper) && !ignoreRight) {
      cBHit = false;
      respRightBumper();
      wallHits = 0;
    }
    stop(30);
  }
}

void setup() {
  
  Serial.begin(9600);  // Set the baud rate to match the Serial Monitor
  Serial.println("hello world!");


  pinMode(rightForward, OUTPUT);
  pinMode(rightBack, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftBack, OUTPUT);
  pinMode(leftBumper, INPUT_PULLUP);
  pinMode(rightBumper, INPUT_PULLUP);
  pinMode(centerBumper, INPUT_PULLUP);
  balldrop.attach(servoPin);
  // always write your servo to 180 when testing, this is where carlos and I have glues our servo
  balldrop.write(0);

  // setting the speeds for the respective motors
  analogWrite(speedLeft, 230);
  analogWrite(speedRight, 235);
}


// I did not use the state switch, opting for more direct boolean control, as the course is extremely linear
void loop() {
  // this part is used to check whether the initial bumper has been hit. SIDEA == right bumper & closer to the garage.
  // SIDEB == left bumper and farther from the garage.
  while(start_condition == false) {
    if (!digitalRead(rightBumper)) {
      start_condition = true;
      delay(1000);
      side = SIDEA;
    }
    if (!digitalRead(leftBumper)) {
      start_condition = true;
      delay(1000);
      side = SIDEB;
    }
    // tells the robot what to enter next
    spin = true;
  }

  while (spin && start_condition) {

    // read the current peak from the beacon pin
    curPeak = analogRead(beaconPin);
    // spin a certain way based off the side of the map
    if (side == SIDEA) {
      turnRight(20);
    }
    if (side == SIDEB) {
      turnLeft(20);
    }
    // if the curPeak is bigger than our highest recorded peak, and we haven't done a full rotation, update the maxPeak
    if (maxPeak < curPeak && numTurns < spin_threshold) {
      maxPeak = curPeak;
    }
    stop(50);
    // this keeps track of how far we have spun/ once we have done a full rotation
    numTurns ++;

    // if we have done a full rotation and our curPeak is very close to our recorded max, it is time to stop spinning and start driving
    // IF THERE ARE PROBLEMS IN THIS SECTION IT IS ALMOST CERTAINLY HERE!
    if (curPeak >= maxPeak - 20 && numTurns >= spin_threshold) {
      spin = false;
      stop(50);
    if (side == SIDEA) {
      turnRight(123);
    }
    if (side == SIDEB) {
      turnLeft(123);
    }
      break;
    }
  }

  // this section is the initial wall following algorithm to get out of the initial two walls
  while(!outOfBox && numTurns > 2) {
    // once the center bumper has been hit at least once and the robot has not hit a wall for a little bit it is time to turn and go into
    // the main map. THIS IS WHERE ISSUES MIGHT ARRISE FOR THIS SECTION!! IF IT IS NOT WORKING CHECK THIS PART, SPECIFICALLY WALLHITS
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

      drive(19);

      if (side == SIDEA) {
        turnLeft(200);
      }
      if (side == SIDEB) {
        turnRight(200);
      }
      findingZone = true;
      // we might want to add a break here. It was working without it but could be a good edition
    }

    // these two sections are a simple wall following algorithm, essentially, ignore the bumper for the side of the robot where the wall isn't
    // (if you are following a wall on your left, ignore the right bumper) and then drive forward, occasionally turning back at the wall
    if (side == SIDEB) {
      // ignore the bumper not in use
      ignoreRight = true;
      // drive forward
      drive(5);
      // turn back towards the wall you are following
      turnLeft(20);
    }

    if (side == SIDEA) {
      ignoreLeft = true;
      drive(5);
      turnRight(20);
    }
    // wallhits is reset in the drive command, based off whether or not it hits a bumper
    wallHits ++;
    // check to see if the center bumper is hit
    if (!digitalRead(centerBumper) || cBHit) {
      bigHit = true;
      wallHits = 0;
      // reverse to get some space off the wall
      reverse(110);
      // turn to face our next wall and continue to follow it
      if (side == SIDEA) {
        turnLeft(260);
      }
      if (side == SIDEB) {
        turnRight(260);
      }
    }
  }

  // this section is used to find the contact zone, it is the part where billy is facing it and kind of ramming the wall, backing up
  // turning a little, then ramming it again, until he hits his center bumper
  while(findingZone) {
    // reset which bumpers to ignore !IMPORTANT!
    ignoreRight = false;
    ignoreLeft = false;
    forward(20);
    stop(30);
    // if the center bumper is it, we know we have correctly alligned on the wall and it is time to progress
    if (!digitalRead(centerBumper)) {
      contactZone = true;
      // backup and orient on the next wall
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
    // if a given bumper is hit, back up and turn towards it, then drive forward again.
    if (!digitalRead(rightBumper)) {
      reverse(50);
      turnRight(50);
    }
    if (!digitalRead(leftBumper)) {
      reverse(50);
      turnLeft(50);
    }
  }

  // this section occurs once the contact zone it hit, it simply drive along the next wall until the center bumper is hit again,
  // triggering the ball drop
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

    // once the center bumper is hit
    if(!digitalRead(centerBumper) || cBHit) {
      // drop the balls
      while(true) {
        // turn off the motors
        digitalWrite(leftForward, 0);
        digitalWrite(rightForward, 0);

        digitalWrite(leftBack, 0);
        digitalWrite(rightBack, 0);

        // drop the balls, important to put the 180 loop first!!
        for (int i = 180; i >= 0; i--) {
          balldrop.write(i);
          delay(5); // Adjust delay as needed for your servo speed
        }
        for (int i = 0; i <= 180; i++) {
          balldrop.write(i);
          delay(5); // Adjust delay as needed for your servo speed
          }
      }
    }
  }  
}
