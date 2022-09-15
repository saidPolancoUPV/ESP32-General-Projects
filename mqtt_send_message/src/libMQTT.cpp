#include "libQMTT.h"

void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i=0; i < length; i++) {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    // aquí se ponen los eventos que se quieran manejar
    /*********************** Ejemplo **********************************************************/
    /*
        if (String(topic) == "esp32/output") {
            Serial.print("Changing output to ");
            if(messageTemp == "on"){
                Serial.println("on");
                digitalWrite(ledPin, HIGH);
            } else if(messageTemp == "off"){
                Serial.println("off");
                digitalWrite(ledPin, LOW);
            }
        }
    */
    /******************************************************************************************/
}

void initMQTT(PubSubClient &client, const char * mqtt_server) {
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void reconnectMQTT(PubSubClient &client, const String *list_of_topics) {
    // clico hasta que este conectado
    while (!client.connected()) {
        Serial.print("Conectando a Servidor MQTT...");
        if (client.connect("ESP32Client", "spolancom", "m15cuy1t0s")) {
            Serial.println("connected");
            // subscribe
            client.subscribe("/esp32/test");

            for (byte i=0; i < 2; i++) {
                Serial.println(list_of_topics[i]);
            }
        } else {
            Serial.print("Error en la conección, rc=");
            Serial.print(client.state());
            Serial.println(" intentando en 5 segundos");
            delay(5000);
        }
    }
}