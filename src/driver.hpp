#ifndef _DRIVER_HPP
#define _DRIVER_HPP

#include "stdint.h"
#include "IntervalTimer.h"

class Driver{
public:
    Driver(uint8_t stepPin, uint8_t dirPin, uint8_t sleepPin);
    int64_t getNumberOfSteps();
    void enable();
    void disable();
    bool isEnabled();
    void setMaxSpeed(uint32_t maxSpeed); //In step per minute
    void setCurrentSpeed(int64_t currentSpeed); //In step per minute

private:
    volatile int64_t nbOfSteps;
    bool enabled;
    uint32_t maxSpeed;
    int64_t currentSpeed;
};

#endif /* _DRIVER_HPP */
