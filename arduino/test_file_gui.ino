#include <AccelStepper.h>
#include <ezButton.h>

const int dirPin = 2;
const int stepPin = 3;

const int but_num = 24;

const int en = 4;
const int forward = 6;
const int backward = 5;


int speeed = 255;

ezButton but(but_num);

// Define motor interface type
#define motorInterfaceType 1

// Creates an instance
AccelStepper stepper(motorInterfaceType, stepPin, dirPin);

int state = 1;

int outByte = 'T';


int incomingByte;
bool left = false;
bool right = false;

bool up = false;
bool down = false;

void stopLA() {
  digitalWrite(forward, LOW);
  digitalWrite(backward, LOW);
}

void moveLinearAct(bool forwards) {
  if (forwards == true) {
    digitalWrite(forward, HIGH);
    digitalWrite(backward, LOW);
  } else {
    digitalWrite(forward, LOW);
    digitalWrite(backward, HIGH);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  stepper.setMaxSpeed(4000); //SPEED = Steps / second
  stepper.setAcceleration(4000); //ACCELERATION = Steps /(second)^2 
 
  pinMode(forward, OUTPUT);//set relay as an output
  pinMode(backward, OUTPUT);//set relay as an output


  analogWrite(en, speeed);
  digitalWrite(forward, HIGH);
  digitalWrite(backward, HIGH);
  but.setDebounceTime(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  but.loop();
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    
    if (incomingByte == 'L') {
        if (!left) {
          right = false;
          stepper.setSpeed(4000);
          left = true;
        } 
      stepper.runSpeed();
    } else if (incomingByte == 'R') {
        if (!right) {
          left = false;
          stepper.setSpeed(-4000);
          right = true;
        }
      stepper.runSpeed();
    } 
    
    if (incomingByte == 'U') {
        moveLinearAct(true);
    } 
    else if (incomingByte == 'D') {
        moveLinearAct(false);
    } 
    else if (incomingByte == 'N')
    {
        stopLA();
    }
    
    if (incomingByte == 'C') {
      state = 2;
    }
  } if (Serial.availableForWrite() > 0) {
      if (but.getCount() > 1) {
        Serial.write(outByte);
      }
  }
}
