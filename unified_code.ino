// create servo object to control a servo
#include <Servo.h>
#include <Adafruit_VL6180X.h>
#include <AccelStepper.h>

// definitions
#define motorInterfaceType 1

/*
==================================================================================================================
---------------------------------------------FILE INFO------------------------------------------------------------
==================================================================================================================
*/

/*
This file contains the code to perform all the movement within the robot. That includes 
*/

// COORDINATE SYSTEM //

/* 
x: up and down the arm
y: along the width of the arm 
z: distance from the rail to the arm
*/

// STATE DEFINITIONS //

/*
state 1: moving the linear slide up and down to perform initial scan
state 2: moving the linear slide into its x position
state 3: rotating the first servo into the y position
state 4: moving the linear actuator into position
state 5: moving the second servo motor into position
state 6: moving the linear slide, actuator, and servo motor while inserting the needle
*/


/*
==================================================================================================================
---------------------------------------------PHYSICAL QUANTITIES--------------------------------------------------
==================================================================================================================
*/

// the depth of the vein from the surface of the skin (determined by the ultrasound image and length/pixel approximations)
int h_us = 10;

// placeholder final coordinates (these points theoretically determined w the ultrasound and the hypothetical comp vis model)
int xf, yf, zf = 1;

float l3 = 1.00;

/*
==================================================================================================================
---------------------------------------------VARIABLES------------------------------------------------------------
==================================================================================================================
*/

////////////// MOTORS //////////////

// current state of the robot
int state = 0;

// starting positions 
int x, y, z;

// setting the pins for the linear actuator
const int forward = 38;
const int backward = 39;

// creating objects to move servo motors
Servo myservo;
Servo myservo1;

const int servo_1_pin = 30;
const int servo_2_pin = 31;

// creating variables for the linear slide
const int dirPin = 22;
const int stepPin = 23;

AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

////////////// SENSORS //////////////

//vl6180x (shorter range on linear actuator)
Adafruit_VL6180X v1 = Adafruit_VL6180X();

// Limit switches
const int first_switch = 46;
const int sec_switch = 47;

///////////////// MISC. ///////////////////

// Reading of sensor on linear slide to determine how close it is to the further end
float range_2_reading;
float length_from_end;

/*
==================================================================================================================
-----------------------------------------------CALCULATIONS-------------------------------------------------------
==================================================================================================================
*/

float l1 = 1.0;
float l2 = 1.0;
float q1 = 1.0;
float q2 = 1.0;

/*
==================================================================================================================
---------------------------------------------CUSTOM FUNCTIONS-----------------------------------------------------
==================================================================================================================
*/

/*!
  @brief: Deactivate both relays to brake the motor
*/
void stopLA() {
    digitalWrite(forward, HIGH);
    digitalWrite(backward, HIGH);
}

/*!
  @brief: Moving the motor. putting true into the function extends the actuator
  @arg int forward: boolean on wither you want it to move forward
  @return: nothing
*/
void moveLinearAct(bool forwards) {
  if (forwards == true) 
  {
    digitalWrite(forward, HIGH);
    digitalWrite(backward, LOW);
  } 
  else 
  {
    digitalWrite(forward, LOW);
    digitalWrite(backward, HIGH);
  }
}

/*
==================================================================================================================
------------------------------------------------SETUP-------------------------------------------------------------
==================================================================================================================
*/

void setup() {
  
  //////////////////////////// SETTING PINS ////////////////////////////

  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(servo_1_pin);

  // Second servo motor connected to pin 3
  myservo1.attach(servo_2_pin);
  
  // setting pins associated with the actuator
  pinMode(forward, OUTPUT); //set relay as an output
  pinMode(backward, OUTPUT); //set relay as an output

  // establishing settings for stepper
	pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  digitalWrite(dirPin, LOW);

  // pins for further range ToF sensors
  // vl61
  v1.begin();

  // pins for the buttons
  pinMode(first_switch, INPUT);
  pinMode(sec_switch, INPUT);
  digitalWrite(dirPin, HIGH);

  // Setting up pins for the 
  pinMode(first_switch, INPUT);
  pinMode(sec_switch, INPUT);

  //////////////////// SETTING EVERYTHING TO NEUTRAL POSITIONS ////////////////////
  
  // Set both motors to their neutral positions
  myservo.write(90);
  myservo1.write(0); 
}

/*
==================================================================================================================
-------------------------------------------------LOOP-------------------------------------------------------------
==================================================================================================================
*/

void loop() {
  switch (state) 
  {
    case 0:
      // needs rising edge trigger, so these three lines will cause one turn
      digitalWrite(stepPin, LOW);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(60);

      // checks if limit switch has been pressed
      if (digitalRead(first_switch) == HIGH) 
      {
        state = 1;
        digitalWrite(dirPin, HIGH); // flips the direction of slide and switches state to 1
      }

    case 1:

      digitalWrite(stepPin, LOW);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(60);

      if (digitalRead(sec_switch) == HIGH) // !!!!!! CHANGE THIS TO SECOND SWITCH AFTER YOU GET THE WIRES YOU NEED FOR IT !!!!!!! //
      {
        state = 2;
      }

    case 2: // still need to think about how I am going to get the distance along the slide (use the vl53 or calculate the distance per signal/loop to get distance)
      state = 3; // UPDATE THIS LATER!
    case 3:
      
      myservo.write(q1);

    case 4:

      if (v1.isRangeComplete()) 
      {
        if (v1.readRangeResult() > h_us) 
        {
          moveLinearAct(true);
        }

        while (v1.readRangeResult() > h_us) 
        {
          delay(10);
        }

        stopLA();

        state = 5;
      }

    case 5:

      myservo1.write(q2);

    case 6: // you need to beter define this!
      break;
  }
}
