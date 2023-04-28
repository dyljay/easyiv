#include <Wire.h>
#include <Servo.h>

// creating objects to move servo motors
Servo myservo;

int angle = 0;
int angle2 = 180;

void setup() {
  // The servo control wire is connected to Arduino D2 pin.
  myservo.attach(6);

}

void loop() {
  myservo.write(angle);
  delay(1000);

  myservo.write(angle2);
  delay(1000);
}
