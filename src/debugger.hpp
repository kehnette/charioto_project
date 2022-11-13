#ifndef _DEBUGGER_HPP
#define _DEBUGGER_HPP

#include "stdint.h"

#include "Arduino.h"

void debuggerInit();
void debuggerBlue(uint8_t level);
void debuggerRed(uint8_t level);  //for errors
void debuggerGreen(uint8_t level);
void debuggerBlack(uint8_t level);

#endif /* _DEBUGGER_HPP */
