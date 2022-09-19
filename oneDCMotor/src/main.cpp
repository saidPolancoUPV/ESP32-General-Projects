#include <Arduino.h>
#include "CMotor.h"


CMotor motor(30000, 0, 8, 27, 26, 14);
CMotor motor2(30000, 2, 8, 33, 32, 25);

void setup() {
  Serial.begin(115200);

  motor.init();
  motor2.init();

  // testing
  Serial.print("Testing DC Motor...");
}

void loop() {
  motor.forward();
  motor2.forward();
  delay(1000);

  motor.stop();
  motor2.stop();
  
  delay(5000);
}