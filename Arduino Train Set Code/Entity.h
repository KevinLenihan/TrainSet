/*
  Entity.h - Abstract base class
  Created By Kevin Lenihan
*/
#ifndef Entity_h
#define Entity_h

#include "Arduino.h"

// list of interrupt commands
enum interrupt_t {STOP, GO, CHANGE_DIRECTION_FWD, CHANGE_DIRECTION_REV, SPEED_UP, SLOW_DOWN, 
  SLOW_DOWN_INTERSECTION, DELAY, LIGHT_GREEN, LIGHT_RED, RANDOM_STOP, OBEY_LIGHT, OBEY_LIGHT_PHILLY, 
  WAITING, FIRST, FIRST_CLEAR};

class Entity{
  public:
    // abtract method that is used for detector driven and delayed events
    virtual void interrupter(int int_code) = 0;
};

#endif