#include "blinker.hpp" //Import header, because it is here that the class is defined.
#include "Arduino.h"

Blinker::Blinker(){
    //Empty constructor
}

//Constructors
Blinker::Blinker(uint8_t pinBlink){
    this->pinBlink = pinBlink;
    pinMode(pinBlink,OUTPUT);
    pinMode(9,OUTPUT);
    disable();

}

//Setters
void Blinker::setBlinkPeriod(uint32_t blinkPeriod){
    this->blinkPeriod = blinkPeriod;
}

void Blinker::setBlinkTimeOn(uint32_t blinkTimeOn){
    this->blinkTimeOn = blinkTimeOn;
}

void Blinker::setBlinkPerPeriod(uint32_t nbBlinks)
{
    this->nbBlinks=nbBlinks;
}

void Blinker::setTimeBetweenBlinks(uint32_t timeBetweenBlinks)
{
    this->timeBetweenBlinks=timeBetweenBlinks;
}

void Blinker::enable(){
    T0=millis();
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
    digitalWrite(9,LOW);
    ledOn = false;
}

void Blinker::powerLED(){
    digitalWrite(pinBlink,HIGH);
    digitalWrite(9,HIGH);
    if (!ledOn)
        millisLastRisingOn = millis(); //The led was OFF, so it is a rising edge
    ledOn = true;
}

//Updater
void Blinker::update(){
    if (enabled){
        uint32_t m = millis();
        if (m>T0+blinkPeriod)
        {
            T0=m;
        }
        for (int N=0 ; N<nbBlinks ; N++)
        {
            uint32_t startN=T0+N*timeBetweenBlinks;
            uint32_t endN=startN+blinkTimeOn;
            if (m<endN && m>startN)
            {
                //Serial.print("ON seg");Serial.println(N);
                powerLED();
            }
        }
        for (int N=1 ; N<nbBlinks ; N++)
        {
            uint32_t startN=T0+N*timeBetweenBlinks;
            uint32_t endNm1=startN-(timeBetweenBlinks-blinkTimeOn);
            if (m<startN && m>endNm1)
            {
                //Serial.print("OFF seg");Serial.println(N);
                shutDownLED();
            }
        }
        if (m>T0+(nbBlinks-1)*timeBetweenBlinks+blinkTimeOn && m<T0+blinkPeriod)
        {
            //Serial.print("OFF final");
            shutDownLED();
        }
    }
}