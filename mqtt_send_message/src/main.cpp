#include <Arduino.h>
#include "common.h"
#include "libWiFi.h"
#include "libQMTT.h"

// establecemos credenciales de acceso a WiFi
const char* ssid = "los odio a todos";
const char* password = "p1ch1C0ntr453n4F4@";

const char* mqtt_server = "192.168.0.110";

unsigned long previous_time = 0;
unsigned long time_delay = 20000; // 20 segundos

String list_of_topics[] = {"/esp32/test", "/esp32/test2"};

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  initWiFi(ssid, password);
  initMQTT(client, mqtt_server);
}

void loop() {
  if (!client.connected()) reconnectMQTT(client, list_of_topics);
  client.loop();

  int randInt = random(0, 100);

  client.publish("/esp32/test", String(randInt).c_str());

  reconexion();
  delay(3000);
}