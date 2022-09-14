#include <Arduino.h>
#include "CWiFi.h"

CWiFi my_wifi(String("los odio a todos"), String("p1ch1C0ntr453n4F4@"), true);

void setup() {
  Serial.begin(115200);
  my_wifi.init();
}

void loop() {
  my_wifi.reconnect();
  delay(500);
}