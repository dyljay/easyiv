#include <AccelStepper.h>
#include <Servo.h>
#include <Adafruit_VL6180X.h>

double startt;
double currt;
double prevt = 0.0;

const int dirPin = 2;
const int stepPin = 3;

const int en = 9;
const int forward = 10;
const int backward = 11;

double cfs = 8. / 400.;
double todeg = 180. / PI;
double torad = PI / 180.;


double q1 = 83 * torad;
double k = sin(q1) / cos(q1);
double vs = 1600 * cfs;
double q2 = 45 * torad;
double r1 = 80*cos(q2);

double speeed = 60;

double q1f;
double q2f;

double timeframe = 10;

Adafruit_VL6180X v1 = Adafruit_VL6180X();


double a = 50;
double b = -100;
double c = 60;

Servo myservo;
Servo myservo1;

AccelStepper stepper(1, stepPin, dirPin);

bool firstpass = true;


double calcangle(double t){
  double angle = q1 - asin(((2*vs*t)+(2*r1*q1))/(k*r1));

  return angle;
}

double calculateangle(double t) {
  double angle = (a*t*t) + (b*t) + c;
  
  return angle;
}

void stopLA() {
  digitalWrite(forward, LOW);
  digitalWrite(backward, LOW);
}

int state = 1;

void moveLinearAct(bool forwards) {
  if (forwards == true) {
    digitalWrite(forward, HIGH);
    digitalWrite(backward, LOW);
  } else {
    digitalWrite(forward, LOW);
    digitalWrite(backward, HIGH);
  }
}

void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
  Serial.begin(9600);

  myservo.attach(12); // motor on top
  myservo1.attach(13);

  stepper.setMaxSpeed(4000); //SPEED = Steps / second
  stepper.setAcceleration(4000); //ACCELERATION = Steps /(second)^2 

  pinMode(forward, OUTPUT);//set relay as an output
  pinMode(backward, OUTPUT);//set relay as an output

  analogWrite(en, 255);
  digitalWrite(forward, HIGH);
  digitalWrite(backward, HIGH);

  v1.begin();
  
  moveLinearAct(false);
  delay(1000);
  stopLA();

  analogWrite(en, 50);
  
  delay(1000);
  myservo.write(45);
  delay(1000);

  myservo1.write(20);
  delay(1500);

  delay(10000);
}

void loop() {
  // put your main code here, to run repeatedly:  
  switch (state) {
    case 1:
      if (v1.readRange() > 130) 
      {
        moveLinearAct(true);
      }
      else 
      {
        stopLA();
        state = 2;
      }
      break;
    case 2:
      if (currt < 1.) 
      {
          if (firstpass > 0) 
          {
            startt = millis();
            stepper.move(1800);
            stepper.setSpeed(1600);
            firstpass = false;
          }
    
          //myservo.write((4*calcangle(currt)));
          stepper.runSpeed();
          myservo1.write(calculateangle(currt)-15);
      }
      else if (currt < 3.)
      {
        stopLA();  
        stepper.runSpeed();
      }
    
      currt = millis();
      currt -= startt;
      currt /= 1000;
      break;
 }
 
  
  /*
  if (!isnan(calcangle(currt))) 
  {
    if (firstpass > 0) {
      startt = millis();
      firstpass = false;
    }
  
    currt = millis();
    currt -= startt;
    currt /= 1000;

    Serial.println(currt);
    Serial.println(calculateangle(currt));
  
    prevt = currt;
  
    stepper.run();
  }
  */
}
