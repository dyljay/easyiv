
// setting the pins for the linear actuator
const int forward = 8;
const int backward = 9;

// Custom function to stop the linear actuator to reduce redundancies in code
void stopLA() {
  digitalWrite(forward, HIGH);
  digitalWrite(backward, HIGH);
}

// method to move the linear actuator in a particular direction
void moveLinearAct(bool forward) {
    if (forward == true) {
      digitalWrite(forward, HIGH);
      digitalWrite(backward, LOW);
    } else {
      digitalWrite(forward, LOW);
      digitalWrite(backward, HIGH);
    }
}


void setup() {
  // setting pins associated with the actuator
  pinMode(forward, OUTPUT);//set relay as an output
  pinMode(backward, OUTPUT);//set relay as an output
}

void loop() {
  // moving the linear actuator forward, waiting 2 secs, and then stopping the motor
  moveLinearAct(true);
  delay(2000);
  stopLA();

  // moving LA backwards, waiting 2 seconds, then stopping again for good
  moveLinearAct(false);
  delay(2000);
  stopLA();

}
