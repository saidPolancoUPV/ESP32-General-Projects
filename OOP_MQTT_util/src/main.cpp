#include <Arduino.h>
#include "CPubSub.h"

CPubSub ps(String("INFINITUMD80E_2.4"), String("uADQZq7ZAQ"), String("192.168.0.13"), true);

void setup() {
  Serial.begin(115200);
  ps.init();
}

void loop() {
  ps.loop();
  delay(1000);
}