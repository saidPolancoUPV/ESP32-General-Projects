/************** Definicion de librerías ***************************/
#include <Arduino.h>
#include "CWiFi.h"
#include <PubSubClient.h>
#include "CMotor.h"
/*******************************************************************/

/****************** Definición de constantes ***********************/
const char *ssid = "los odio a todos";
const char *passwd = "p1ch1C0ntr453n4F4@";
// const char *mqtt_server = "192.168.0.106";
IPAddress mqttServer(192, 168, 0, 106);

String list_of_topics[] = {"/esp32/motor1", "/esp32/motor2"};
int num_of_topics = 2;

int frequency = 30000;
int resolution = 8;
/*******************************************************************/

void reconnectMQTT();
void callback(char* topic, byte* message, unsigned int length);


CMotor motor1(frequency, 0, resolution, 27, 26, 14);
CMotor motor2(frequency, 2, resolution, 33, 32, 25);
CWiFi wifi(ssid, passwd); // iniciamos el objeto CWiFi

WiFiClient espClient; //cliente WiFi
PubSubClient mqttClient(mqttServer, 1883, callback, espClient); // PubSubClient para el manejo de mensajes MQTT


void setup() {
  Serial.begin(115200);
  wifi.init();

  motor1.init();
  motor2.init();
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

    if (String(topic) == "/esp32/motor1") {
      if (messageTemp == "ON") {
        motor1.forward();
      }

      if (messageTemp == "OFF") {
        motor1.stop();
      }
    }

    if (String(topic) == "/esp32/motor2") {
      if (messageTemp == "ON") {
        motor2.forward();
      }

      if (messageTemp == "OFF") {
        motor2.stop();
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