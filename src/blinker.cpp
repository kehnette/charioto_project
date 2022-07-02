#include "blinker.hpp" //Import header, because it is here that the class is defined.
#include "Arduino.h"

Blinker::Blinker(){
    //Empty constructor
}

//Constructors
Blinker::Blinker(uint8_t pinBlink){
    this->pinBlink = pinBlink;
    pinMode(pinBlink,OUTPUT);;
    disable();

}

//Setters
void Blinker::setBlinkPeriod(uint32_t blinkPeriod){
    this->blinkPeriod = blinkPeriod;
}

void Blinker::setBlinkTimeOn(uint32_t blinkTimeOn){
    this->blinkTimeOn = blinkTimeOn;
}

void Blinker::enable(){
    enabled = true; //this-> is mandatory only when there might be a confusion. Writing "this->" can be useful to let VSC show you all your class attributes
    this->millisLastRisingOn = millis();
    powerLED();
}

void Blinker::disable(){
    enabled = false;
    shutDownLED();
}

bool Blinker::isEnabled(){
    return enabled;
}

//Getters
uint32_t Blinker::getBlinkPeriod(){
    return blinkPeriod;
}

uint32_t Blinker::getBlinkTimeOn(){
    return blinkTimeOn;
}

void Blinker::shutDownLED(){
    digitalWrite(pinBlink,LOW);
    ledOn = false;
}

void Blinker::powerLED(){
    digitalWrite(pinBlink,HIGH);
    if (!ledOn)
        millisLastRisingOn = millis(); //The led was OFF, so it is a rising edge
    ledOn = true;
}

//Updater
void Blinker::update(){
    if (enabled){
        uint32_t m = millis();
        if (ledOn && m-millisLastRisingOn>blinkTimeOn)
            shutDownLED();
        else if (!ledOn && m-millisLastRisingOn>blinkPeriod)
            powerLED();
    }  
}