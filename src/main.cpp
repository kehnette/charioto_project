#include <Arduino.h>
#include <pins.hpp>
#include "math.h"
#include "driver.hpp"
#include "debugger.hpp"

Driver* leftMot;
Driver* rightMot;
int i = 0;
uint64_t millisLedOn = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(PIN_BATT,INPUT);

  Serial.println("Left");
  leftMot = new Driver(PIN_MOTOR_G_STEP,PIN_MOTOR_G_DIR,PIN_MOTOR_ENABLED,25000,Driver::ESIDES::LEFT);
  leftMot->setCurrentSpeed(500);
  leftMot->enable();

  Serial.println("Right");
  rightMot = new Driver(PIN_MOTOR_D_STEP,PIN_MOTOR_D_DIR,PIN_MOTOR_ENABLED,50000,Driver::ESIDES::RIGHT);
  rightMot->setCurrentSpeed(500);
  rightMot->enable();
  debuggerInit();
}

void loop() {
  debuggerBlue((i%2==0)?HIGH:LOW);
  // put your main code here, to run repeatedly:

  
  float period = 5; //s
  float amplitude = 5000; //tick/s
  float x = sin(2*3.1415/period*millis()/1000)*amplitude;
  leftMot->setCurrentSpeed(x);
  rightMot->setCurrentSpeed(x*0.9);
  //Serial.println(x);

  

  if (i%500==0)
    Serial.println(analogRead(PIN_BATT)/1023.0*16.6);

  if (millis()-millisLedOn<100)
    digitalWrite(LED_BUILTIN,HIGH);
  else
    digitalWrite(LED_BUILTIN,LOW);

  
  if (millis()-millisLedOn>1000){
    digitalWrite(LED_BUILTIN,HIGH);
    millisLedOn=millis();
  }
  
  i++;

  delay(1);
}