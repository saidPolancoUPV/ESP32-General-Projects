#include <Arduino.h>
#include "CWiFi.h"
#include <PubSubClient.h>

/****************** Definición de constantes ***********************/
const char *ssid = "los odio a todos";
const char *passwd = "p1ch1C0ntr453n4F4@";
// const char *mqtt_server = "192.168.0.106";
IPAddress mqttServer(192, 168, 0, 106);

String list_of_topics[] = {"/cactuscare/ventilador", "/cactuscare/foco", "/cactuscare/placaterm"};
int num_of_topics = 3;

int pinVentilador = 19;
int pinFoco = 21;
int pinPlacaTerm = 22;
/***********************************************************************************************************/

/****************** Definición de prototipos *****************************************************************/
void reconnectMQTT();
void callback(char* topic, byte* message, unsigned int length);
/***********************************************************************************************************/

/****************** Definición de objetos globales *********************************************************/
CWiFi wifi(ssid, passwd); // iniciamos el objeto CWiFi
WiFiClient espClient;
PubSubClient mqttClient(mqttServer, 1883, callback, espClient);
/***********************************************************************************************************/

void setup() {
  Serial.begin(115200);
  wifi.init();


  pinMode(pinVentilador, OUTPUT);
  pinMode(pinFoco, OUTPUT);
  pinMode(pinPlacaTerm, OUTPUT);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  wifi.reconexion();
  delay(500);
}

void callback(char* topic, byte* message, unsigned int length) {
    /* Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: "); */
    String messageTemp;

    for (int i=0; i < length; i++) {
         // Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }

    if (String(topic) == "/cactuscare/ventilador") {
      if (messageTemp == "ON") {
        digitalWrite(pinVentilador, HIGH);
      }

      if (messageTemp == "OFF") {
        digitalWrite(pinVentilador, LOW);
      }
    }

    if (String(topic) == "/cactuscare/foco") {
      if (messageTemp == "ON") {
        digitalWrite(pinFoco, HIGH);
      }

      if (messageTemp == "OFF") {
        digitalWrite(pinFoco, LOW);
      }
    }

    if (String(topic) == "/cactuscare/placaterm") {
      if (messageTemp == "ON") {
        digitalWrite(pinPlacaTerm, HIGH);
      }

      if (messageTemp == "OFF") {
        digitalWrite(pinPlacaTerm, LOW);
      }
    }

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

void reconnectMQTT() {
    // clico hasta que este conectado
    while (!mqttClient.connected()) {
        Serial.print("Conectando a Servidor MQTT...");
        if (mqttClient.connect("ESP32Client", "spolancom", "m15cuy1t0s")) {
            Serial.println("connected");
            // subscribe
            // mqttClient.subscribe("/esp32/test");

            for (byte i=0; i < num_of_topics; i++) {
                mqttClient.subscribe(list_of_topics[i].c_str());
            }
        } else {
            Serial.print("Error en la conección, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" intentando en 5 segundos");
            delay(5000);
        }
    }
}