// Include the AccelStepper Library
#include <AccelStepper.h>

// conversion ratio from turns to distance
// 400 steps = 1 revolution = 8 mm
float conversion_turns_to_mm = 400. / 8.;
float conversion_mm_to_turns = 8. / 400.;

// Define pin connections
const int dirPin = 8;
const int stepPin = 9;

// Define motor interface type
#define motorInterfaceType 1

// Creates an instance
AccelStepper stepper(motorInterfaceType, stepPin, dirPin);

/*
void RotateRelative()
{
    //We move X steps from the current position of the stepper motor in a given direction.
    //The direction is determined by the multiplier (+1 or -1)
   
    runallowed = true; //allow running - this allows entering the RunTheMotor() function.
    stepper.setMaxSpeed(receivedSpeed); //set speed
    stepper.move(directionMultiplier * receivedSteps); //set relative distance and direction
}

void RotateAbsolute()
{
    //We move to an absolute position.
    //The AccelStepper library keeps track of the position.
    //The direction is determined by the multiplier (+1 or -1)
    //Why do we need negative numbers? - If you drive a threaded rod and the zero position is in the middle of the rod...
 
    runallowed = true; //allow running - this allows entering the RunTheMotor() function.
    stepper.setMaxSpeed(receivedSpeed); //set speed
    stepper.moveTo(directionMultiplier * receivedSteps); //set relative distance   
}
*/

int state = 1;
const int first_sw = 24;
const int sec_sw = 25;

/*
void move_slide_arb(int dir) {
  if (dir) 
  {

  }
  else 
  {

  }


}
*/

void slide_speed(int dir, int speed) {
  stepper.setSpeed(dir * speed);
}

void setup() {
  Serial.begin(9600);
	// set the maximum speed, acceleration factor,
  pinMode(first_sw, INPUT_PULLUP);
  pinMode(sec_sw, INPUT_PULLUP);
  
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  // Set the spinning direction CW/CCW:
  digitalWrite(dirPin, LOW);
  
  /*
  stepper.setMaxSpeed(1000); //SPEED = Steps / second
  stepper.setAcceleration(800); //ACCELERATION = Steps /(second)^2 
  stepper.setSpeed(400);
  */
}

bool truther = false;

void loop() {
  //stepper.enableOutputs();
  //stepper.moveTo(0);
  Serial.println(state);
  switch (state) {
    case 1:
      if (digitalRead(first_sw) == HIGH)
      {
        digitalWrite(stepPin, LOW);
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500);
      }
      else 
      { 
        digitalWrite(dirPin, HIGH);
        if (truther)
        {
          state = 3;
        } 
        else 
        {
          state = 2;
        }
      }
      
      break;

    case 2:
      if (digitalRead(sec_sw) == HIGH)
      { 
        digitalWrite(dirPin, LOW);
        state = 1;
        truther = true;
      }

      digitalWrite(stepPin, LOW);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(500);

      break;

    case 3:
      if (digitalRead(first_sw) == HIGH)
      {
        digitalWrite(stepPin, LOW);
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500);
      }
      else 
      { 
        state = 4;
      }

      break; 
      
    case 4:
      break;
  }
}