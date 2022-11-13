#include <Arduino.h>
#include "blinker.hpp"

Blinker blk;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  blk = Blinker(LED_BUILTIN);
  blk.setBlinkPeriod(10000);
  blk.setBlinkTimeOn(10000);
  blk.setBlinkPerPeriod(1);
  blk.setTimeBetweenBlinks(0);
  blk.enable();

  //Emptying serial buffer
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  while (Serial.available()>0){//I can read something on the serial bus
    String s = Serial.readString();

    if (s.equalsIgnoreCase("ON")){ 
      blk.enable();
      Serial.println("START BLINK");
    }

    else if (s.equalsIgnoreCase("OFF")){
      blk.disable();
      Serial.println("STOP BLINK");
    }
    
  }

  blk.update();
  delay(0.2);
  
}