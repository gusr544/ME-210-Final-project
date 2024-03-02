#include <Servo.h>

Servo balldrop;

void setup() {
  Serial.begin(9600);
  balldrop.attach(A1);
  balldrop.write(180);
  delay(1000);
}

void loop() {
  // Spin servo from 0 to 180 degrees
        for (int i = 180; i >= 0; i--) {
          balldrop.write(i);
          delay(5); // Adjust delay as needed for your servo speed
        }
        for (int i = 0; i <= 180; i++) {
          balldrop.write(i);
          delay(5); // Adjust delay as needed for your servo speed
          }
}
