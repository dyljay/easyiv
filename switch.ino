/*
    ARDUINO LIMIT SWITCH TUTORIAL: NORMALLY CLOSED
    By: TheGeekPub.com
    More Arduino Tutorials: https://www.thegeekpub.com/arduino-tutorials/
*/
 
#define LIMIT_SWITCH_PIN 24
 
void setup() {
  Serial.begin(9600);
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
}
 
void loop() {
 
  if (digitalRead(LIMIT_SWITCH_PIN) == LOW)
  {
    Serial.println("First Activated!");
  }
 
  else if (digitalRead(25) == LOW)
  {
    Serial.println("Second Activated!");
  }

  else 
  {
    Serial.println("None Activated.");
  }
   
  delay(100);
}