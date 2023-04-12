#include <AccelStepper.h>

const int dirPin = 8;
const int stepPin = 9;

const int forward = 38;
const int backward = 39;


// Define motor interface type
#define motorInterfaceType 1

// Creates an instance
AccelStepper stepper(motorInterfaceType, stepPin, dirPin);

int state = 1;

int incomingByte;
bool left = false;
bool right = false;

bool up = false;
bool down = false;

void stopLA() {
  digitalWrite(forward, HIGH);
  digitalWrite(backward, HIGH);
}

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
  // put your setup code here, to run once:
  Serial.begin(9600);

  stepper.setMaxSpeed(400); //SPEED = Steps / second
  stepper.setAcceleration(1600); //ACCELERATION = Steps /(second)^2 
  stepper.setSpeed(400);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    if (incomingByte == 'L') {
        if (!left) {
          right = false;
          stepper.setSpeed(400);
          left = true;
        } 
      stepper.runSpeed();
    } else if (incomingByte == 'R') {
        if (!right) {
          left = false;
          stepper.setSpeed(-400);
          right = true;
        }
      stepper.runSpeed();
    } 
    
    if (incomingByte == 'U') {
        down = false;
        moveLinearAct(false);
        up = true;
    } else if (incomingByte == 'D') {
        up = false;
        moveLinearAct(true);
        down = true;
    } 

    if (incomingByte == 'C') {
      state = 2;
    }
  }
}
