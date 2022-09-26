#include <Arduino.h>
#include "CWiFi.h"
#include <PubSubClient.h>

/****************** Definición de constantes ***********************/

int tmpPin = 21;
const char *ssid = "los odio a todos";
const char *passwd = "p1ch1C0ntr453n4F4@";
// const char *mqtt_server = "192.168.0.106";
IPAddress mqttServer(192, 168, 0, 106);

String list_of_topics[] = {"/esp32/soil"};
int num_of_topics = 1;

int soilSendorPin = 34;

unsigned long previousValue = 0;
unsigned long currentValue;
/*******************************************************************/

void reconnectMQTT();
void callback(char* topic, byte* message, unsigned int length);
float getPercentHumidity(int soil_sensor_val);

CWiFi wifi(ssid, passwd); // iniciamos el objeto CWiFi

WiFiClient espClient; //cliente WiFi
PubSubClient mqttClient(mqttServer, 1883, callback, espClient); // PubSubClient para el manejo de mensajes MQTT

void setup() {
  Serial.begin(115200);
  wifi.init();

  pinMode(tmpPin, OUTPUT);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  digitalWrite(tmpPin, HIGH);
  delay(100);
  int value = analogRead(soilSendorPin);
  delay(100);
  digitalWrite(tmpPin, LOW);

  currentValue = millis();

  /* if ((currentValue - previousValue) > 10000) {
    previousValue = currentValue;
    mqttClient.publish("/esp32/soil", String(getPercentHumidity(value)).c_str());
  } */
  Serial.print("Sensor: ");
  Serial.println(value);
  Serial.print("Percent: ");
  Serial.println(getPercentHumidity(value));

  wifi.reconexion();
  delay(2800);
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

float getPercentHumidity(int soil_sensor_val) {
    int tmp_val = soil_sensor_val - 1090; // el 1300 sólo es a prueba y error de acuerdo al sensor.

    float tmp_value = ((3005.0 - float(tmp_val)) / (3005.0 - 0.0));

    return tmp_value;
}