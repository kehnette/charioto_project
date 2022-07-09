#include "driver.hpp"
#include "IntervalTimer.h"
#include "Arduino.h"
#include <algorithm>
#include "debugger.hpp"

Driver* Driver::listOfDriversToUpdate[2] = {nullptr,nullptr};
IntervalTimer Driver::intTimer;
uint32_t Driver::timeClockInterval = 1000000; //microseconds
bool Driver::intTimerStarted = false;

Driver::Driver(uint8_t stepPin, uint8_t dirPin, uint8_t sleepPin, uint32_t maxSpeed, ESIDES side){
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->sleepPin = sleepPin;
    setMaxSpeed(maxSpeed);
    setCurrentSpeed(0);
    this->side = side;
    this->enabled = false;

    listOfDriversToUpdate[(uint8_t)side] = this;
}

int64_t Driver::getNbOfSteps(){
    noInterrupts();
    int64_t out = nbOfSteps;
    interrupts();

    return out;
}

void Driver::enable(){
    noInterrupts();

    enabled = true;
    currentNbOfTicks = 0;

    interrupts();
    
    digitalWrite(sleepPin,HIGH);
    if (!Driver::intTimerStarted){
        
        bool success = intTimer.begin(&Driver::updateAllDrivers,Driver::timeClockInterval);
        if (success){
            Serial.print("Timer started with period "); 
            Serial.println(Driver::timeClockInterval);
        }
        else{
            Serial.print("Timer failed with period ");
            Serial.println(Driver::timeClockInterval);
        }
        Driver::intTimerStarted = true;
    }
}

void Driver::disable(){
    noInterrupts();

    enabled = false;

    interrupts();

    digitalWrite(sleepPin,LOW);
}

bool Driver::isEnabled(){
    return enabled;
}

void Driver::updateAllDrivers(){
    debuggerRed(HIGH);
    for (int d=0;d<2;d++){
        if (listOfDriversToUpdate[d] != nullptr && listOfDriversToUpdate[d]->isEnabled() && listOfDriversToUpdate[d]->currentSpeed != 0){
            
            Driver* driver = listOfDriversToUpdate[d];
            driver->currentNbOfTicks++;
            if (driver->currentNbOfTicks > driver->currentNbOfClockTicksBetweenRisingEdges)
                driver->currentNbOfTicks = 0;

            if (driver->currentNbOfTicks == 0){
                driver->step();
            }
            else if (driver->currentNbOfTicks == 1)
                driver->release();
        }
    }
    debuggerRed(LOW);
}

uint32_t Driver::currentMicrosBetweenSteps(){
    return 1000000/abs(this->currentSpeed);
}

uint32_t Driver::minMicrosBetweenSteps(){
    return 1000000/this->maxSpeed;
}

void Driver::setMaxSpeed(uint32_t maxSpeed){//In step per second
    this->maxSpeed = maxSpeed;
    uint32_t maxClockPeriod = minMicrosBetweenSteps()/2;
    if (Driver::timeClockInterval > maxClockPeriod){
        Serial.print("I need a more refined clock period : ");Serial.println(maxClockPeriod);
        Driver::timeClockInterval = maxClockPeriod;
    }
    else
        Serial.print("default clock period ok : ");Serial.println(Driver::timeClockInterval);
} 

void Driver::setCurrentSpeed(int64_t currentSpeed){//In step per second
    
    if (currentSpeed>maxSpeed)
        currentSpeed=maxSpeed;

    if (currentSpeed == this->currentSpeed)
        return;

    noInterrupts();

    this->currentSpeed = currentSpeed;
    if (currentSpeed != 0){
        this->currentNbOfClockTicksBetweenRisingEdges = this->currentMicrosBetweenSteps()/Driver::timeClockInterval;

        if (currentSpeed>0){
            clockwise = true;
            digitalWrite(dirPin,HIGH);
        }
        else{
            clockwise = false;
            digitalWrite(dirPin,LOW);
        }
    }
    interrupts();

    Serial.print("Setting currentNbOfClockTicksBetweenRisingEdges at");Serial.println(this->currentNbOfClockTicksBetweenRisingEdges);
    Serial.println((uint32_t)&this->currentNbOfClockTicksBetweenRisingEdges);
} 

void Driver::step(){
    digitalWrite(stepPin,HIGH);
    debuggerGreen(HIGH);
    nbOfSteps += (clockwise?1:(-1));
}

void Driver::release(){
    digitalWrite(stepPin,LOW);
    debuggerGreen(LOW);
}