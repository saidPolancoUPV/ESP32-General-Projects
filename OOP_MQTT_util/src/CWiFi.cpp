#include "CWiFi.h"

CWiFi::CWiFi(String ssid, String password, bool verbose=true) {
    this->VERBOSE = verbose;
    this->SSID = ssid;
    this->PASSWORD = password;
    this->_previous_time = 0;
    this->_time_delay = 20000;
}

void CWiFi::init() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->SSID.c_str(), this->PASSWORD.c_str());
    
    if (this->VERBOSE) {
        Serial.print("Conectando a la Red: ");
        Serial.print(this->SSID);
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
        }
        
        Serial.println("Se ha establecido la conección\n");
        Serial.print("Dirección IP: ");
        Serial.println(WiFi.localIP());
    }

    Serial.println("Done");
}

void CWiFi::reconnect() {
    unsigned long current_time = millis();

    if ((WiFi.status() != WL_CONNECTED) && (current_time - this->_previous_time >= this->_time_delay)) {
        if (this->VERBOSE) {
            Serial.println(millis());
            Serial.println("Reconectando a la red.");
        }
        WiFi.disconnect();
        WiFi.reconnect();

        if (this->VERBOSE) {
            if (WiFi.status() == WL_CONNECTED) Serial.println("Se ha reestablecido la conexión");
        }
        this->_previous_time = current_time;
    }
}

void CWiFi::set_time_delay(unsigned long td) {
    this->_time_delay = td;
}