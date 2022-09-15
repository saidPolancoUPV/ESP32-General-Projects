#include <Arduino.h>
#include "CPubSub.h"

// CPubSub ps(String("INFINITUMD80E_2.4"), String("uADQZq7ZAQ"), String("192.168.0.13"), true);
CPubSub ps(String("los odio a todos"), String("p1ch1C0ntr453n4F4@"), String("192.168.0.110"), true);

void setup() {
  Serial.begin(115200);

  ps.set_mqtt_user(String("spolancom"));
  ps.set_mqtt_password(String("m15cuy1t0s"));

  ps.init();
}

void loop() {
  ps.loop();
  delay(1000);
}