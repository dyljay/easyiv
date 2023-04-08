// create servo object to control a servo
#include <Servo.h>
#include <Adafruit_VL6180X.h>
#include <Adafruit_VL53L0X.h>

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
state 1: moving the linear slide up and down
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
int h_us = 2;

// placeholder final coordinates (these points theoretically determined w the ultrasound and the hypothetical comp vis model)
int xf, yf, zf = 1;

/*
==================================================================================================================
---------------------------------------------VARIABLES------------------------------------------------------------
==================================================================================================================
*/

// MOTORS //

// current state of the robot
int state = 0;

// starting positions 
int x, y, z;

// setting the pins for the linear actuator
const int forward = 8;
const int backward = 9;

// creating objects to move servo motors
Servo myservo;
Servo myservo1;

int servo_1_pin = 2;
int servo_2_pin = 3;

// creating the pins for the linear slide
int pin1;
int pin2;
int pin3;
int pin4;


// SENSORS //

//vl6180x (shorter range on linear actuator)
Adafruit_VL6180X v1 = Adafruit_VL6180X();

//vl5380x (on linear slide for longer distances)
Adafruit_VL53L0X v2 = Adafruit_VL53L0X();
Adafruit_VL53L0X v3 = Adafruit_VL53L0X();


// OTHER VARIABLES //

// Reading of sensor on linear slide to determine how close it is to the further end
float range_2_reading;
float length_from_end;

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
void moveLinearAct(bool forward) {
  if (forward == true) 
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

void moveLinearSlide(int dir, int pul) {

}

/*
==================================================================================================================
------------------------------------------------SETUP-------------------------------------------------------------
==================================================================================================================
*/

void setup() {
  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(servo_1_pin);

  // Second servo motor connected to pin 3
  myservo1.attach(servo_2_pin);

  // Set both motors to their neutral positions
  myservo.write(90);
  myservo1.write(0);
  
  // setting pins associated with the actuator
  pinMode(forward, OUTPUT);//set relay as an output
  pinMode(backward, OUTPUT);//set relay as an output

  // setting the pins for the linear slide
  

  // pins for further range ToF sensors
  // vl61
  v1.begin();
  // vl53's (x2)
  v2.startRangeContinuous();
  v3.startRangeContinuous();
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
      VL53L0X_RangingMeasurementData_t measure;

      if (v2.isRangeComplete()) 
      {
        range_2_reading = v2.readRange();
        length_from_end = v2.readRange();
      }

      state = 1;

    case 1:
      
      if (v2.isRangeComplete()) // do i need to check if it is complete every time? (I should just check if the switches have been hit to know that I've hit the end, don't need to use this)
      {
        range_2_reading = v2.readRange();
      }




      delay(50);
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}
