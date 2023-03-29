/*
    Servo Motor file for basic movement
*/

#include <Servo.h>

// create servo object to control a servo

Servo myservo1;

int pos = 0;    // variable to store the servo position
int pin = 0;    // pin number

void setup() {
    myservo1.attach(pin);  
}

void loop() {

    myservo1.write(pos); // moves motor 1
}