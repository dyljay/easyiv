#include <Servo.h>

Servo myservo;
Servo myservo1;

float star = 0.0;
float en = 0.0;

int q1 = 180;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  myservo.attach(6); // 
  //myservo1.attach(7, 637, 2493); // motor on top of actuator moving in y along width of arm

  myservo.write(60);
  //myservo1.write(90);
}

void loop() {

}
