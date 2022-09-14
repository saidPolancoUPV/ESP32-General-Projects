#include <Arduino.h>
#include <WiFi.h>

// establecemos credenciales de acceso a WiFi
const char* ssid = "los odio a todos";
const char* password = "p1ch1C0ntr453n4F4@";

unsigned long previous_time = 0;
unsigned long time_delay = 20000; // 20 segundos

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a la red: ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Se ha establecido conección.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconexion() {
  unsigned long current_time = millis(); // milisegundos desde la actualización

  // checamos que este desconectado
  if ((WiFi.status() != WL_CONNECTED) && (current_time - previous_time >= time_delay)) {
    Serial.println(millis());
    Serial.println("Reconectando a la red.");
    WiFi.disconnect();
    WiFi.reconnect();

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Se ha reestablecido la conexión");
    }
    previous_time = current_time;
  }
}

void setup() {
  Serial.begin(115200);
  initWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:

  reconexion();
  delay(1000);
}