#include <Wire.h>

// creating objects to move servo motors
Servo myservo;
Servo myservo1;

int angle;

void setup() {
  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(2);

  // Second servo motor connected to pin 3
  myservo1.attach(3);

  Wire.begin(1);
  Wire.onReceieve(RotateMotors);

}

void loop() {
  
  delay(100);
}

void RotateMotors() {
  
  angle = Wire.read();
  myservo1.write(angle);
}

