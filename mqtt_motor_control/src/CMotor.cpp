#include "CMotor.h"

CMotor::CMotor(int frequency, int channel, int resolution, int pin1, int pin2, int enable_pin) {
    this->freq = frequency;
    this->pwmChannel = channel;
    this->resolution = resolution;
    this->max_res = pow(2, resolution) - 1;
    this->motorPin_1 = pin1;
    this->motorPin_2 = pin2;
    this->enablePin = enable_pin;
}

void CMotor::init() {

    pinMode(this->motorPin_1, OUTPUT);
    pinMode(this->motorPin_2, OUTPUT);
    pinMode(this->enablePin, OUTPUT);

    ledcAttachPin(this->enablePin, this->pwmChannel);
    ledcSetup(this->pwmChannel, this->freq, this->resolution);
}

void CMotor::forward() {
    Serial.println("Moving Forward motor");
    digitalWrite(this->motorPin_1, LOW);
    digitalWrite(this->motorPin_2, HIGH); 
    ledcWrite(this->pwmChannel, this->max_res);
}

void CMotor::forward(int mills) {
    Serial.print("Moving Forward motor for ");
    Serial.print(mills);
    Serial.println(" millis");
    digitalWrite(this->motorPin_1, LOW);
    digitalWrite(this->motorPin_2, HIGH); 
    ledcWrite(this->pwmChannel, this->max_res);

    delay(mills);
    this->stop();
}

void CMotor::backward() {
    Serial.println("Moving backward motor");
    digitalWrite(this->motorPin_1, HIGH);
    digitalWrite(this->motorPin_2, LOW); 
    ledcWrite(this->pwmChannel, this->max_res);
}

void CMotor::stop() {
    Serial.println("Stoping motor");
    digitalWrite(this->motorPin_1, LOW);
    digitalWrite(this->motorPin_2, LOW); 
    ledcWrite(this->pwmChannel, 0);
}

