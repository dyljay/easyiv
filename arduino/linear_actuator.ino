// setting the pins for the linear actuator
const int forward = 28;
const int backward = 29;

// Custom function to stop the linear actuator to reduce redundancies in code
void stopLA() {
  digitalWrite(forward, HIGH);
  digitalWrite(backward, HIGH);
}

// method to move the linear actuator in a particular direction
void moveLinearAct(bool forwards) {
  if (forwards == true) {
    digitalWrite(forward, LOW);
    digitalWrite(backward, HIGH);
  } else {
    digitalWrite(forward, HIGH);
    digitalWrite(backward, LOW);
  }
}


void setup() {
  // setting pins associated with the actuator
  pinMode(forward, OUTPUT);//set relay as an output
  pinMode(backward, OUTPUT);//set relay as an output

  digitalWrite(forward, HIGH);
  digitalWrite(backward, HIGH);
}

void loop() {
  moveLinearAct(true);
  delay(2000);
  stopLA();

  moveLinearAct(false);
  delay(2000);
  stopLA();
  // moving LA backwards, waiting 2 seconds, then stopping again for good
}
