#include <Arduino.h>
#include <DHT_U.h>
#include <DHT.h>
#include "CMotor.h"

#define DHT_SENSOR_TYPE DHT22

/****************** Definición de constantes ***********************/
int frequency = 30000;
int resolution = 8;

byte pinActivadorSoilSensor = 21;
byte pinLecturaSoilSensor = 34;
byte pinTempeaturaHumedadSensor = 19;
byte pinUVSensor = 15;
byte REF_3V3_UVSensor = 4;

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
/*******************************************************************/

/******************* Definición de variables globales **************/
DHT dht_sensor(pinTempeaturaHumedadSensor, DHT_SENSOR_TYPE);
CMotor motorInundacion(frequency, 0, resolution, 27, 26, 14);
CMotor motor2(frequency, 2, resolution, 33, 32, 25);

unsigned long tiempoPrevioSoilSensor = 0;
unsigned long tiempoActualSoilSensor;
long tiempoEsperaSoilSensor = 60000;

float valorHumedadTierra;
float valorHumedadAmbiente;
float valorTemperaturaAmbiente;
/*******************************************************************/

void setup() {
  Serial.begin(115200);
  dht_sensor.begin();
  motorInundacion.init();

  pinMode(pinActivadorSoilSensor, OUTPUT);
  pinMode(pinUVSensor, INPUT);
  pinMode(REF_3V3_UVSensor, INPUT);
}

void loop() {
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
  }
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