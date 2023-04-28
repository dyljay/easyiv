// create servo object to control a servo
#include <Servo.h>
#include <Adafruit_VL6180X.h>
#include <AccelStepper.h>
#include <ezButton.h>

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
uint8_t h_us = 6;

// placeholder final coordinates (these points theoretically determined w the ultrasound and the hypothetical comp vis model)
int xf, yf, zf = 1;

// length of the needle
float l3 = 1.00;

// distance (mm) per 1 rotation of the slide
int dist_slide = 8;

// steps per revolution
int steps = 400;

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
const int en_us = 4;
const int forward_us = 5;
const int backward_us = 6;

const int en = 9;
const int forward = 10;
const int backward = 11;                            /*! TODO: CHANGE PINS*/

// creating objects to move servo motors
Servo myservo;
Servo myservo1;

const int servo_1_pin = 12;
const int servo_2_pin = 13;

// creating variables for the linear slide
const int dirPin = 2;
const int stepPin = 3;

AccelStepper stepper(motorInterfaceType, stepPin, dirPin);

// conversion ratio from turns to distance
// 400 steps = 1 revolution = 8 mm
float conversion_turns_to_mm = 400. / 8.;
float conversion_mm_to_turns = 8. / 400.;

////////////// SENSORS //////////////

//vl6180x (shorter range on linear actuator)
Adafruit_VL6180X v1 = Adafruit_VL6180X();

// Limit switches
ezButton but1(8); // first switch to pin 24
ezButton but2(7); // second switch to pin 25

///////////////// MISC. ///////////////////

// variables to keep track of the number of steps the motor takes from one end of the slide to the other
int pos_end1 = 0;
int pos_end2 = 0;

// variable to read the serial to determine movement
int incomingByte;

// logic variables to not let the user go move the slide in both directions simultaneously so nothing is broken lol
bool left = false;
bool right = false;

bool up = false;
bool down = false;


double cfs = 8. / 400.;
double todeg = 180. / PI;
double torad = PI / 180.;


double q11 = 83 * torad;
double k = sin(q11) / cos(q11);
double vs = 1600 * cfs;
double q21 = 45 * torad;
double r1 = 80*cos(q21);

double startt;
double currt;

double a = 50;
double b = -100;
double c = 60;

int outByte = 'T';

bool firstpass = true;

/*
==================================================================================================================
-----------------------------------------------CALCULATIONS-------------------------------------------------------
==================================================================================================================
*/

//double l2 = zf - l20 - h_us;
double q1 = 45; //asin(h_us1/l3);
double q2 = 20; //atan((l3 * (cos(asin(h_us1/l3)))/yf));
double l1 = xf - (l3 * cos(q2) * sin(q1));

/*
==================================================================================================================
---------------------------------------------CUSTOM FUNCTIONS-----------------------------------------------------
==================================================================================================================
*/
double calcangle(double t){
  double angle = q1 - asin(((2*vs*t)+(2*r1*q1))/(k*r1));

  return angle;
}

double calculateangle(double t) {
  double angle = (a*t*t) + (b*t) + c;
  
  return angle;
}

int calc_turns(float length) {
  return pos_end1 - (length * conversion_turns_to_mm);
}

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

void moveLinearActUS(bool forwards) {
  if (forwards == true) 
  {
    digitalWrite(forward_us, HIGH);
    digitalWrite(backward_us, LOW);
  } 
  else 
  {
    digitalWrite(forward_us, LOW);
    digitalWrite(backward_us, HIGH);
  }
}

void stopLAUS() {
    digitalWrite(forward_us, HIGH);
    digitalWrite(backward_us, HIGH);
}

/*
==================================================================================================================
------------------------------------------------SETUP-------------------------------------------------------------
==================================================================================================================
*/

void setup() {
  //////////////////////////// SETTING PINS ////////////////////////////
  Serial.begin(9600);
  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(servo_1_pin);

  // Second servo motor connected to pin 3
  myservo1.attach(servo_2_pin);
  
  // setting pins associated with the actuator
  analogWrite(en_us, 255);
  pinMode(forward, OUTPUT); //set relay as an output
  pinMode(backward, OUTPUT); //set relay as an output

  analogWrite(en, 50);
  pinMode(forward_us, OUTPUT); //set relay as an output
  pinMode(backward_us, OUTPUT); //set relay as an output

  // starting the ToF sensor
  v1.begin();

  // Setting time switch needs to be down to count as a press
  but1.setDebounceTime(50);
  but2.setDebounceTime(50);

  // setting the states for the linear slide
  stepper.setMaxSpeed(4000); //SPEED = Steps / second
  stepper.setAcceleration(4000); //ACCELERATION = Steps /(second)^2 
}

/*
==================================================================================================================
-------------------------------------------------LOOP-------------------------------------------------------------
==================================================================================================================
*/

void loop() {
  but1.loop();
  but2.loop();

  // giving a way out if something breaks
  if (but1.getCount() > 2) {
    state = 99;
  }

  switch (state) 
  {
    case 0:
      //////////////////// SETTING EVERYTHING TO NEUTRAL POSITIONS ////////////////////
      
      // Set both motors to their neutral positions
      myservo.write(45);
      delay(1000);

      myservo1.write(0); 
      delay(1000);
      
      // moving the linear actuator to neutral position
      moveLinearAct(false);
      delay(1500);
      stopLA();

      if (Serial.available() > 0) 
      {
        if (incomingByte == 'B') 
        {
            state = 1;  
        }
      }

      break;

    case 1:

      if (Serial.available() > 0) 
      {

        incomingByte = Serial.read();

        // if/else if block to that governs the movement of the slide.
        if (incomingByte == 'L') 
        {
            if (!left) {
              right = false;
              stepper.setSpeed(4000);
              left = true;
            } 
          stepper.runSpeed();
        } 
        else if (incomingByte == 'R') 
        {
            if (!right) {
              left = false;
              stepper.setSpeed(-4000);
              right = true;
            }
          stepper.runSpeed();
        } 
        
        // governs the movement of the actuator moving the ultrasound
        if (incomingByte == 'U') 
        {   
            moveLinearActUS(false);
        } 
        else if (incomingByte == 'D') 
        {
            moveLinearActUS(true);
        } 
        else if (incomingByte == 'N') 
        {
            stopLAUS();
        }

        // if user decides to go onto the next step of scannning, we end the serial communication, save the position and up the state
        if (incomingByte == 'C') 
        {
          pos_end1 = stepper.currentPosition();

          stepper.setSpeed(1600);

          state = 2;
        }

      }

      break;

    case 2:

      if (but2.isPressed() > 0) 
      {
        if (Serial.availableForWrite() > 0) 
        {
            Serial.write(outByte);
        }
        pos_end2 = stepper.currentPosition();
        stepper.moveTo(-1 * (400/8) * l1);                           /*! TODO: define where it'll move to */
        state = 3;
        delay(1000);
      }

      stepper.runSpeed();

      break;

    case 3:

      if (myservo.read() != q1) 
      {
        myservo.write(myservo.read() + 1);
      }
      else 
      {
        state = 4;
      }

      break;
    
    case 4:

      if (v1.readRange() > 130) 
      {
        moveLinearAct(true);
      }
      else 
      {
        stopLA();
        state = 5;
      }

      break;

    case 6:
      if (Serial.available() > 0) 
      {
        incomingByte = Serial.read();

        if (incomingByte == 'Z')
        {
          state = 8;
        }
      }

      break;

    case 7: // insertion motion
       if (currt < 1.) 
      {
          if (firstpass > 0) 
          {
            startt = millis();
            stepper.move(2000);
            stepper.setSpeed(4000);
            firstpass = false;
          }
    
          moveLinearAct(true);
       
          currt = millis();
          currt -= startt;
          currt /= 1000;
    
          myservo.write((4*calcangle(currt)) + 45);
          myservo1.write(90 + calculateangle(currt));
          stepper.runSpeed();
      }
      else
      {
        stopLA();
        state = 8;  
      }

      break;
      
    default:

      break;
  }
}
