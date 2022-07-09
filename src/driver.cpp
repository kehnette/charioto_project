#include "driver.hpp"
#include "IntervalTimer.h"
#include "Arduino.h"
#include <algorithm>
#include "debugger.hpp"

Driver* Driver::listOfDriversToUpdate[MAX_NB_OF_DRIVERS] = {nullptr,nullptr};
IntervalTimer Driver::intTimer;
uint32_t Driver::timeClockInterval = 1000000; //microseconds
bool Driver::intTimerStarted = false;

Driver::Driver(uint8_t stepPin, uint8_t dirPin, uint8_t sleepPin, uint32_t maxSpeed, ESIDES side){
    //Absolute max speed calculation:
    //An intervalTimer can achive a min period of 1 µs
    //I need 2 ticks to drive a motor (1 high 1 low)
    //maxSpeed is at a period of 2 µs => 500 000 Hz
    this->stepPin = stepPin; pinMode(stepPin,OUTPUT);
    this->dirPin = dirPin; pinMode(dirPin,OUTPUT);
    this->sleepPin = sleepPin; pinMode(sleepPin,OUTPUT);
    setMaxSpeed(maxSpeed);
    setCurrentSpeed(0);
    this->side = side;
    this->enabled = false;

    listOfDriversToUpdate[(uint8_t)side] = this;
}

int64_t Driver::getNbOfSteps(){
    noInterrupts();
    debuggerBlack(HIGH);
    int64_t out = nbOfSteps;
    debuggerBlack(LOW);
    interrupts();

    return out;
}

void Driver::enable(){
    noInterrupts();
    debuggerBlack(HIGH);

    enabled = true;
    currentNbOfTicks = 0;

    debuggerBlack(LOW);
    interrupts();
    
    digitalWriteFast(sleepPin,HIGH);
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
    debuggerBlack(HIGH);

    enabled = false;

    debuggerBlack(LOW);
    interrupts();

    digitalWriteFast(sleepPin,LOW);
}

bool Driver::isEnabled(){
    return enabled;
}

void Driver::updateAllDrivers(){
    debuggerRed(HIGH);
    for (int d=0;d<MAX_NB_OF_DRIVERS;d++){
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
        if (Driver::intTimerStarted)
            Driver::intTimer.update(Driver::timeClockInterval); // We need to recompute existing currentNbOfClockTicksBetweenRisingEdges
    }
    else{
        Serial.print("default clock period ok : ");Serial.println(Driver::timeClockInterval);
    }
    Driver::recomputeCurrentNbOfClockTicksBetweenRisingEdges();
}

void Driver::setCurrentSpeed(int64_t currentSpeed){//In step per second
    if (currentSpeed>maxSpeed)
        currentSpeed=maxSpeed;

    noInterrupts();
    debuggerBlack(HIGH);

    if (this->currentSpeed*currentSpeed<0){
        if(currentSpeed>0){
            clockwise = true;
            digitalWriteFast(dirPin,HIGH);
        }
        else{
            clockwise = false;
            digitalWriteFast(dirPin,LOW);
        }
    }

    this->currentSpeed = currentSpeed;
    if (currentSpeed != 0)
        this->currentNbOfClockTicksBetweenRisingEdges = this->currentMicrosBetweenSteps()/Driver::timeClockInterval;

    debuggerBlack(LOW);
    interrupts();

}

void Driver::recomputeCurrentNbOfClockTicksBetweenRisingEdges(){
    noInterrupts();
    debuggerBlack(HIGH);
    for (int d=0;d<MAX_NB_OF_DRIVERS;d++){
        if (listOfDriversToUpdate[d] != nullptr ){
            Serial.print("Recomputing");Serial.println(d);
            uint32_t currentSpeed = listOfDriversToUpdate[d]->currentSpeed;
            listOfDriversToUpdate[d]->setCurrentSpeed(currentSpeed);
        }
    }
    debuggerBlack(LOW);
    interrupts();
}

void Driver::step(){
    digitalWriteFast(stepPin,HIGH);
    debuggerGreen(HIGH);
    nbOfSteps += (clockwise?1:(-1));
}

void Driver::release(){
    digitalWriteFast(stepPin,LOW);
    debuggerGreen(LOW);
}