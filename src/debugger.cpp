#include "debugger.hpp"
#include "Arduino.h"

void debuggerInit(){
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);
}

void debuggerBlue(uint8_t level){
    digitalWrite(10,level);
}

void debuggerRed(uint8_t level){
    digitalWrite(9,level);
}

void debuggerGreen(uint8_t level){
    digitalWrite(11,level);
}

void debuggerBlack(uint8_t level){
    digitalWrite(8,level);
}