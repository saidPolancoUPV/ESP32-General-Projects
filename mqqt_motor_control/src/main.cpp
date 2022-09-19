#include <Arduino.h>
#include "CWiFi.h"
#include "libQMTT.h"
#include "CMotor.h"

const char *mqtt_server = "192.168.0.110";

CWiFi myWiFi("los odio a todos", "p1ch1C0ntr453n4F4@");

String list_of_topics[] = {"/esp32/test", "/esp32/test2"};
int num_of_topics = 2;

WiFiClient espClient;
PubSubClient client(espClient);

CMotor motor1(30000, 0, 8, 27, 26, 14);

void setup() {
  Serial.begin(115200);
  myWiFi.init();
  initMQTT(client, mqtt_server);
  motor1.init();
}

void loop() {
  if (!client.connected()) reconnectMQTT(client, list_of_topics);
  client.loop();

  int randInt = random(0, 100);

  myWiFi.reconexion();
  delay(1000);
}