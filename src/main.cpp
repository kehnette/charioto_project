#include <Arduino.h>
#include <pins.hpp>
#include <HardwareSerial.h>
#include <AccelStepper.h>
#include "math.h"
#include <Wire.h>

AccelStepper leftMot;
int i = 0;
uint64_t millisLedOn = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(PIN_MOTOR_G_DIR,OUTPUT);
  //pinMode(PIN_MOTOR_G_STEP,OUTPUT);
  pinMode(PIN_MOTOR_ENABLED,OUTPUT);
  pinMode(PIN_BATT,INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(PIN_MOTOR_ENABLED,HIGH);
  //digitalWrite(PIN_MOTOR_G_DIR,HIGH);
  leftMot = AccelStepper(1,PIN_MOTOR_G_STEP,PIN_MOTOR_G_DIR);
  leftMot.setMaxSpeed(10000);
  leftMot.setSpeed(5000);
  analogReadRes(10);

}

void loop() {
  // put your main code here, to run repeatedly:
  leftMot.runSpeed();
  float period = 5; //s
  float amplitude = 5000; //tick/s
  float x = sin(2*3.1415/period*millis()/1000)*amplitude;
  leftMot.setSpeed(x);
  if (i%5000==0)
    Serial.println(analogRead(PIN_BATT)/1023.0*16.6);

  if (millis()-millisLedOn<20)
    digitalWrite(LED_BUILTIN,HIGH);
  else
    digitalWrite(LED_BUILTIN,LOW);

  
  if (millis()-millisLedOn>1000){
    digitalWrite(LED_BUILTIN,HIGH);
    millisLedOn=millis();
  }
  
  i++;
}


/*
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SERIAL_GPS.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(SERIAL_GPS.readString());
}*/