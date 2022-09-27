#include <Arduino.h>
#include "pins_definition.h"
#include "CWiFi.h"
#include <PubSubClient.h>
#include <DHT_U.h>
#include <DHT.h>
#include "CMotor.h"

/****************** Definición de constantes ***********************/
const char *ssid = "los odio a todos";
const char *passwd = "p1ch1C0ntr453n4F4@";
// const char *mqtt_server = "192.168.0.106";
IPAddress mqttServer(192, 168, 0, 106);

String list_of_topics[] = {"/cactuscare/ventilador", "/cactuscare/foco", "/cactuscare/placaterm", "/cactuscare/values/UV", "/cactuscare/values/humedad", "/cactuscare/values/temperatura", "/cactuscare/values/soil"};
int num_of_topics = 7;

int frequency = 30000;
int resolution = 8;

byte pinActivadorSoilSensor = 23;
byte pinLecturaSoilSensor = 36;
byte pinUVSensor = 39;
byte REF_3V3_UVSensor = 34;

float valorDisparoEncendidoMotor = 0.5; // [0, 1] valor entre 0 y 1 para el encendido del motor.
/*******************************************************************/

/******************* Definición de prototipos ***********************/
int obtenValorSoilSensor();
float calculaPorcentajeDeHumedad(int soil_sensor_val);
bool tiempoDeEspera(unsigned long &valorPrevio, long &tiempoEspera);
void manejoBombaInundacion(float valorHumedad);
void estableceTiempoSensoHumedad(long nuevoValor);
int averageAnalogRead(int pinToRead);
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

void reconnectMQTT();
void callback(char* topic, byte* message, unsigned int length);
/*******************************************************************/

/******************* Definición de variables globales **************/

CWiFi wifi(ssid, passwd); // iniciamos el objeto CWiFi
WiFiClient espClient;
PubSubClient mqttClient(mqttServer, 1883, callback, espClient);

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
CMotor motorInundacion(frequency, 0, resolution, MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_ENABLE_PIN);
CMotor motorNutrientes(frequency, 2, resolution, MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_ENABLE_PIN);

unsigned long tiempoPrevioSoilSensor = 0;
unsigned long tiempoActualSoilSensor;
long tiempoEsperaSoilSensor = 60000;

float valorHumedadTierra;
float valorHumedadAmbiente;
float valorTemperaturaAmbiente;
/*******************************************************************/

void setup() {
  Serial.begin(115200);
  wifi.init();
  dht_sensor.begin();
  motorInundacion.init();

  pinMode(pinActivadorSoilSensor, OUTPUT);
  pinMode(pinUVSensor, INPUT);
  pinMode(REF_3V3_UVSensor, INPUT);

  pinMode(VENTILADOR_PIN, OUTPUT);
  pinMode(FOCO_PIN, OUTPUT);
  pinMode(PLACA_TERMICA_PIN, OUTPUT);
}

void loop() {

  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  if (tiempoDeEspera(tiempoPrevioSoilSensor, tiempoEsperaSoilSensor)) {
    valorHumedadTierra = calculaPorcentajeDeHumedad(obtenValorSoilSensor());
    Serial.print("Porcentaje de humedad: ");
    Serial.println(valorHumedadTierra);
    manejoBombaInundacion(valorHumedadTierra);

    valorHumedadAmbiente = dht_sensor.readHumidity();
    valorTemperaturaAmbiente = dht_sensor.readTemperature();

    if (isnan(valorHumedadAmbiente) || isnan(valorTemperaturaAmbiente)) {
      Serial.println("Fallo en la lecuta del sensor DHT");
    } else {
      Serial.print("Humedad Ambiente: ");
      Serial.println(valorHumedadAmbiente);
      Serial.print("Temperatura Ambiente");
      Serial.println(valorTemperaturaAmbiente);
    }

    int uvLevel = averageAnalogRead(pinUVSensor);
    int refLevel = averageAnalogRead(REF_3V3_UVSensor);

    float outputVoltage = 3.3 / refLevel * uvLevel;
  
    float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
  
    Serial.print("output: ");
    Serial.print(refLevel);
  
    Serial.print("ML8511 output: ");
    Serial.print(uvLevel);
  
    Serial.print(" / ML8511 voltage: ");
    Serial.print(outputVoltage);
  
    Serial.print(" / UV Intensity (mW/cm^2): ");
    Serial.print(uvIntensity);
    Serial.println();

    mqttClient.publish("/cactuscare/values/UV", String(uvIntensity).c_str());
    delay(100);
    mqttClient.publish("/cactuscare/values/humedad", String(valorHumedadAmbiente).c_str());
    delay(100);
    mqttClient.publish("/cactuscare/values/temperatura", String(valorTemperaturaAmbiente).c_str());
    delay(100);
    mqttClient.publish("/cactuscare/values/soil", String(valorHumedadTierra).c_str());
    delay(100);
  }
  wifi.reconexion();
}

void manejoBombaInundacion(float valorHumedad) {
  Serial.print("Estado del motor: ");
  Serial.println(motorInundacion.estaMotorEncendido());
  Serial.print("Valor humedad: ");
  Serial.println(valorHumedad);

  if (!motorInundacion.estaMotorEncendido()) {
    if (valorHumedad < valorDisparoEncendidoMotor) {
      motorInundacion.forward();
      tiempoEsperaSoilSensor = 1000;
    }
  } else { // motor encendido
    if (valorHumedad >= valorDisparoEncendidoMotor) {
      motorInundacion.stop();
      tiempoEsperaSoilSensor = 60000;
    }
  }
}

float calculaPorcentajeDeHumedad(int soil_sensor_val) {
  int valorAjustado = soil_sensor_val - 1090; // el 1300 sólo es a prueba y error de acuerdo al sensor.
  return((3005.0 - float(valorAjustado)) / (3005.0 - 0.0));
}

int obtenValorSoilSensor() {
  digitalWrite(pinActivadorSoilSensor, HIGH);
  delay(100);
  int value = analogRead(pinLecturaSoilSensor);
  delay(100);
  digitalWrite(pinActivadorSoilSensor, LOW);

  return value;
}

bool tiempoDeEspera(unsigned long &valorPrevio, long &tiempoEspera) {
  bool tmpVal = false;
  unsigned long valorActual = millis();

  if ((valorActual - valorPrevio) > tiempoEspera) {
    tmpVal = true;
    valorPrevio = valorActual;
  }

  return tmpVal;
}

int averageAnalogRead(int pinToRead) {
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
 
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
 
  return(runningValue);
} 

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
        digitalWrite(VENTILADOR_PIN, HIGH);
      }

      if (messageTemp == "OFF") {
        digitalWrite(VENTILADOR_PIN, LOW);
      }
    }

    if (String(topic) == "/cactuscare/foco") {
      if (messageTemp == "ON") {
        digitalWrite(FOCO_PIN, HIGH);
      }

      if (messageTemp == "OFF") {
        digitalWrite(FOCO_PIN, LOW);
      }
    }

    if (String(topic) == "/cactuscare/placaterm") {
      if (messageTemp == "ON") {
        digitalWrite(PLACA_TERMICA_PIN, HIGH);
      }

      if (messageTemp == "OFF") {
        digitalWrite(PLACA_TERMICA_PIN, LOW);
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