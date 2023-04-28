#include <Adafruit_VL53L0X.h>

//vl5380x (on linear slide for longer distances)
Adafruit_VL53L0X v2 = Adafruit_VL53L0X();
Adafruit_VL53L0X v3 = Adafruit_VL53L0X();

void setup() {
  // put your setup code here, to run once:
  // vl53's (x2)
  v2.startRangeContinuous();
  v3.startRangeContinuous();
}

void loop() {
  // put your main code here, to run repeatedly:
  VL53L0X_RangingMeasurementData_t measure;
}
