#include "libWiFi.h"

void initWiFi(const char * ssid, const char * passwd) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);
  Serial.print("Conectando a la red: ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
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