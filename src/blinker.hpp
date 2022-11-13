#ifndef _BLINKER_HPP
#define _BLINKER_HPP

#include <stdint.h>

class Blinker{
    //This class is a test which should be used as a reference for Emmy La Mechante in case she has a doublt

public:

    //Constructors
    Blinker();
    Blinker(uint8_t pinBlink); // stdint.h allows for a precise control of the number of bits
    
    //Setters
    void setBlinkPeriod(uint32_t blinkPeriod); // Set the period between blinks (ms)
    void setBlinkTimeOn(uint32_t blinkTimeOn); // Set the time the LED should be ON when blinking (ms)
    /*
        O = On
        x = Off
        OxxxxOxxxxOxxxxOxxxx    is BlinkPeriod of 5ms and a BlinkTimeOn of 1ms
        OOxOOxOOxOOx            is BlinkPeriod of 3ms and a BlinkTimeOn of 2ms
    */
    void setBlinkPerPeriod(uint32_t nbBlinks);
    void setTimeBetweenBlinks(uint32_t timeBetweenBlinks);

    void enable();
    void disable();

    //Getters
    uint32_t getBlinkPeriod();
    uint32_t getBlinkTimeOn();
    bool isEnabled();

    //Updater
    void update(); //To be called as much as possible

private:
    void shutDownLED();
    void powerLED();
    uint8_t pinBlink;
    uint32_t millisLastRisingOn;
    uint32_t blinkPeriod;
    uint32_t blinkTimeOn;
    bool enabled;
    bool ledOn;
    uint32_t nbBlinks;
    uint32_t timeBetweenBlinks;
    uint32_t T0=0;
}; //Ne pas oublier le ;

#endif /* _BLINKER_HPP */
