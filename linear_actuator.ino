#include <Wire.h>
#include <Adafruit_VL6180X.h>

// setting the pins for the linear actuator
const int forward = 50;
const int backward = 51;

// Custom function to stop the linear actuator to reduce redundancies in code
void stopLA() {
  digitalWrite(forward, HIGH);
  digitalWrite(backward, HIGH);
}

Adafruit_VL6180X v1 = Adafruit_VL6180X();

// method to move the linear actuator in a particular direction
void moveLinearAct(bool forwards) {
  if (forwards == true) {
    digitalWrite(forward, LOW);
    digitalWrite(backward, HIGH);
  } else {
    digitalWrite(forward, HIGH);
    digitalWrite(backward, LOW);
  }
}


void setup() {
  Serial.begin(115200);
  v1.begin();
  uint8_t range = v1.readRange();
  uint8_t status = v1.readRangeStatus();
  // setting pins associated with the actuator
  pinMode(forward, OUTPUT);//set relay as an output
  pinMode(backward, OUTPUT);//set relay as an output

  digitalWrite(forward, HIGH);
  digitalWrite(backward, HIGH);
}

void loop() {
  Serial.println(v1.readRange());
  
  if (v1.readRange() > 20 && v1.readRange() < 40) {
    // moving the linear actuator forward, waiting 2 secs, and then stopping the motor
    moveLinearAct(true);
  } else if (v1.readRange() > 40 && v1.readRange() < 80) {
    moveLinearAct(false);
  } else {
    stopLA();
  }
  // moving LA backwards, waiting 2 seconds, then stopping again for good
}
