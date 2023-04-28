#include <Servo.h>

Servo myservo;

double cf = 3.14 / 180.;


double angle1 = 90.0 * cf;
double angle2 = 45 * (3.14 / 180.);

double r2;
double a = 1 / 30;
double b = -1.2;

double va = 2;
double startt;
double currt;
double prevt = 0.0;

bool first_pass = true;

/*
void calcangle(double t) {
    r2 = sin(angle1);

    angle2 += -10 * (((-2*a*r2*vs*cos(angle2)) + (2*a*currt*vs*vs) + b*vs - va) / (r2 * (cos(angle2) - b*sin(angle2) + (2*r2*a*cos(angle2)*sin(angle2)) - (2*a*currt*sin(angle2)*vs)))) * t;    
}
*/
double cfs = 8. / 400.;
double todeg = 180. / PI;
double torad = PI / 180.;


double q1 = 83. * torad;
double k = sin(q1) / cos(q1);
double vs = 1600 * cfs;
double q2 = 45 * torad;
double r1 = 80*cos(q2);

double calcangle(double t){
  double angle = asin(((2*vs*t)+(2*r1*q1))/(k*r1)) / 2.;

  return angle;
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (first_pass > 0) {
    startt = millis();
    first_pass = false;
  }

  currt = millis();
  currt -= startt;
  currt /= 1000;

  Serial.println(calcangle(currt) * todeg);

  delay(100);

  prevt = currt;
}
