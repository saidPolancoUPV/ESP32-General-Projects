#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// establecemos credenciales de acceso a WiFi
const char* ssid = "los odio a todos";
const char* password = "p1ch1C0ntr453n4F4@";

const char* mqtt_server = "192.168.0.110";

unsigned long previous_time = 0;
unsigned long time_delay = 20000; // 20 segundos

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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

void initMQTT() {
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void reconnectMQTT() {
    // clico hasta que este conectado
    while (!client.connected()) {
        Serial.print("Conectando a Servidor MQTT...");
        if (client.connect("ESP32Client", "spolancom", "m15cuy1t0s")) {
            Serial.println("connected");
            // subscribe
            client.subscribe("/esp32/test");
        } else {
            Serial.print("Error en la conección, rc=");
            Serial.print(client.state());
            Serial.println(" intentando en 5 segundos");
            delay(5000);
        }
    }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
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

void setup() {
  Serial.begin(115200);
  initWiFi();
  initMQTT();
}

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  reconexion();
  delay(1000);
}