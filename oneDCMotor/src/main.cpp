#include <Arduino.h>

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14;

int motor2Pin1 = 33;
int motor2Pin2 = 32;
int enable2Pin = 25;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // configure LED PWM functionalitites
  // ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  // ledcAttachPin(enable1Pin, pwmChannel);

  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");
}

void loop() {
  // Move the DC motor forward at maximum speed
  Serial.println("Moving Forward motor 1");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  
  analogWrite(enable1Pin, 255);

  delay(2000);

  Serial.println("stop motor 1");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW); 
  analogWrite(enable1Pin, 0);
  delay(5000);

  Serial.println("Moving Forward motor 2");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); 
  
  analogWrite(enable2Pin, 255);

  delay(2000);

  Serial.println("stop motor 2");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW); 
  analogWrite(enable2Pin, 0);
  delay(5000);
}