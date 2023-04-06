#include <Wire.h>

// current state of the system
int state = 0;

// pins that correspond with the pins to the arduinos that perform the functions
int sensor = 1;
int linear_slide = 2;
int linear_act = 3;
int servo = 4;

void setup() {
  // Begin transmission
  Wire.begin();
}

void loop() {
  // Getting the sensors to turn on
  Wire.beginTransmission(sensor);
  Wire.write(state);
  Wire.endTransmission();

  Wire.beginTransmission(linear_slide);
  Wire.write(state);
  Wire.endTransmission();
}
