/*
This file contains the code to move all the motor systems in the robot.
*/

#include <Servo.h>
// create servo object to control a servo

/*
int x: up and down the arm
int y: along the width of the arm 
int z: distance from the arm
*/

// starting positions 
int x, y, z;

// placeholder final coordinates (these points theoretically determined w the ultrasound and the hypothetical comp vis model)
int xf, yf, zf = 1;

// setting the pins for the linear actuator
const int forward = 8;
const int backward = 9;

// creating objects to move servo motors
Servo myservo;
Servo myservo1;


// creating the pins for the linear slide




/*

Placeholder method to incorporate after i get the laser distance sensors. this will probably be running on another arduino so that it can run in parallel with the movement

vod detectDistance() {

}
*/


//Deactivate both relays to brake the motor
void stopLA() {
  digitalWrite(forward, HIGH);
  digitalWrite(backward, HIGH);
}

void moveLinearAct(bool forward) {
    if (forward == true) {
      digitalWrite(forward, HIGH);
      digitalWrite(backward, LOW);
    } else {
      digitalWrite(forward, LOW);
      digitalWrite(backward, HIGH);
    }
}


// thinking the setup function should include moving the needle to the specific point
void setup() {
  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(2);

  // Second servo motor connected to pin 3
  myservo1.attach(3);

  // Set both motors to their neutral positions
  myservo.write(90);
  myservo1.write(0);
  
  // setting pins associated with the actuator
  pinMode(forward, OUTPUT);//set relay as an output
  pinMode(backward, OUTPUT);//set relay as an output

  // move the linear slide into position
  
}

// loop function should be verifying the insertion with the video footage
void loop() {
  
  // moves first servo motor
  myservo.write(180);
  delay(1000);

  //moves second servo motor
  myservo1.write(180);
  delay(1000);

  // moving the linear actuator forward, waiting 2 secs, and then stopping the motor
  moveLinearAct(true);
  delay(2000);
  stopLA();

  // moving LA backwards, waiting 2 seconds, then stopping again for good
  moveLinearAct(false);
  delay(2000);
  stopLA();

  // moving linear slide

}