#include "IMU_BMI270.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10000);

  Serial.println("Flight controller Starting...\n");
  Wire.begin(8,9);

  if(initializeBMI270(0x69))
  Serial.println("Initialization was successful!");
  else
  Serial.println("Initialization failed :(");

}

void loop() {
  // put your main code here, to run repeatedly:

}
