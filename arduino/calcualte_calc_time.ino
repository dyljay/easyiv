// create servo object to control a servo
#include <Servo.h>
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
uint8_t h_us = 5;
float h_us1 = 5.;

// placeholder final coordinates (these points theoretically determined w the ultrasound and the hypothetical comp vis model)
float xf = 202.; 
float yf = 7.;
float zf = 22.;

// length of the needle
float l3 = 80.;

// distance (mm) per 1 rotation of the slide
int dist_slide = 8;

// steps per revolution
int steps = 400;
float l20 = 3.05;

double del = 0.0000;
double star = 0.0000;
double en = 0.0000;

float q1 = 0.;
float l1 = 0.;
float q2 = 0.;
float l2 = 0.;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  star = millis() * 1000;

  q1 = asin(h_us1/l3);
  
  l2 = zf - l20 - h_us1;
  
  q2 = atan((l3 * (cos(asin(h_us1/l3)))/yf));
  
  l1 = xf - (l3 * cos(q2) * sin(q1));
  
  en = millis() * 1000;

  del = en - star;

  Serial.print("Start: ");
  Serial.println(star);

  Serial.print("End: ");
  Serial.println(en);

  Serial.print("FINAL DELAY: ");
  Serial.println(en - star);
  delay(2000);
}
