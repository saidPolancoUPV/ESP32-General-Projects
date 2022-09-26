#include "CWiFi.h"

CWiFi::CWiFi(const char * ssid, const char * passwd) {
  this->ssid = ssid;
  this->passwd = passwd;

  this->previous_time = 0;
  this->time_delay = 20000; // 20 secs
}

void CWiFi::init() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(this->ssid, this->passwd);
  Serial.print("Conectando a la red: ");
  Serial.println(this->ssid);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Se ha establecido conección.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void CWiFi::reconexion() {

  unsigned long current_time = millis(); // milisegundos desde la actualización

  // checamos que este desconectado
  if ((WiFi.status() != WL_CONNECTED) && (current_time - this->previous_time >= this->time_delay)) {
    Serial.println(millis());
    Serial.println("Reconectando a la red.");
    WiFi.disconnect();
    WiFi.reconnect();

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Se ha reestablecido la conexión");
    }
    this->previous_time = current_time;
  }

}