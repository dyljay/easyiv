#include <AccelStepper.h>
#include <ezButton.h>

// pins connected to
const int dirPin = 8;
const int stepPin = 9;

// Define motor interface type
#define motorInterfaceType 1

// Creates an instance
AccelStepper stepper(motorInterfaceType, stepPin, dirPin);

// pins of switches
ezButton but1(24); // first switch to pin 24
ezButton but2(25); // second switch to pin 25

int state = 1;
int pos_end1 = 0;
int pos_end2 = 0;

void moveStep() {
  stepper.move(500);
}

void setup() {
  // put your setup code here, to run once:
  stepper.setMaxSpeed(400); //SPEED = Steps / second
  stepper.setAcceleration(1600); //ACCELERATION = Steps /(second)^2 
  stepper.setSpeed(-400);

  but1.setDebounceTime(50);
  but2.setDebounceTime(50);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  but1.loop();
  but2.loop();

  switch (state) {
    case 1: 
      if (but1.getCount() == 1) {
        pos_end1 = stepper.currentPosition();
        stepper.setSpeed(-1 * stepper.speed());
        state = 2;
      }
      else {
        stepper.runSpeed();
      }
      
      break;

    case 2:

      if (but2.getCount() == 1) {
        pos_end2 = stepper.currentPosition();
        stepper.setSpeed(-1 * stepper.speed());
        state = 3;
      }

      stepper.runSpeed();

      break;

    case 3:

      if (but1.getCount() == 2) {
        stepper.setSpeed(0);
        moveStep(); // maybe instead of this, you can have a moveTo and just move it an absolute amount of distance so that there is no variability from the first time it reaches one end to the other
        state = 4;
      }

      stepper.runSpeed();

      break;

    case 4:
      
      delay(1250);
      state = 5;
      break;

    case 5:
      if (stepper.distanceToGo() == 0) {
        state = 6;
      }

      stepper.run();
      break;
    
    case 6:
      Serial.println(pos_end1);
      Serial.println(pos_end2);
      state = 7;

    default:
      break;
  }
}
