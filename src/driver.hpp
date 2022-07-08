#ifndef _DRIVER_HPP
#define _DRIVER_HPP

#include "stdint.h"
#include "IntervalTimer.h"

class Driver{
public:
    
    enum ESIDES{LEFT=0,RIGHT=1};

    Driver(){}
    Driver(uint8_t stepPin, uint8_t dirPin, uint8_t sleepPin, uint32_t maxSpeed, ESIDES side);
    int64_t getNbOfSteps();
    void enable();
    void disable();
    bool isEnabled();
    void setMaxSpeed(uint32_t maxSpeed); //In step per second
    void setCurrentSpeed(int64_t currentSpeed); //In step per second
    

private:
    //Params with getters
    bool enabled;
    uint32_t maxSpeed;
    int64_t currentSpeed;
    bool clockwise;
    ESIDES side;

    //PINS
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t sleepPin;

    //ISR
    volatile int64_t nbOfSteps;
    volatile uint32_t currentNbOfClockTicksBetweenRisingEdges;
    volatile uint32_t currentNbOfTicks = 0;
    uint32_t currentMicrosBetweenSteps();
    uint32_t minMicrosBetweenSteps();

    //Common to all drivers
    static Driver* listOfDriversToUpdate[2];
    static IntervalTimer intTimer;
    static bool intTimerStarted;
    static void updateAllDrivers();
    static uint32_t timeClockInterval; //microseconds
    
    void step();
    void release();
};

#endif /* _DRIVER_HPP */
