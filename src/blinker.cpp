#include "blinker.hpp" //Import header, because it is here that the class is defined.
#include "Arduino.h"

//Constructors
Blinker::Blinker(uint8_t pin_blink){
    this->pin_blink = pin_blink;
    pinMode(pin_blink,OUTPUT);
    enabled = false;
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
    digitalWrite(pin_blink,HIGH);
}

void Blinker::disable(){
    enabled = false;
    digitalWrite(pin_blink,LOW);
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

//Updater
void Blinker::update(){
    if (enabled){
        uint32_t m = millis();
        
    }
    
}