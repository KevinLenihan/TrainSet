/*
  updater.h - Sends a periodic interrupt to resend engine speed commands
  Created By Kevin Lenihan
*/
#ifndef Updater_h
#define Updater_h

#include "Arduino.h"
#include "Commander.h"
#include "Entity.h"
#include "Rail.h"
#include "Engine.h"

class Updater {
  public:
    Updater();
    void addEngine(Engine* eng);
    void update();
    
  private:
    Engine* engines[MAX_NUM_RAILS];
    int size;
    int index;
};

#endif