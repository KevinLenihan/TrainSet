/*
  Engine.h - defines Engine Entity
  Created By Kevin Lenihan
*/

#ifndef Engine_h
#define Engine_h

#include "Arduino.h"
#include "Commander.h"
#include "Entity.h"

const int SPEED_DELAY = 1000;

typedef struct Speeds{
  int min;
  int average;
  int max;
};

class Engine: public Entity {
  public:
    int _dir;
    int _id;

    Engine(Commander* cmder, int id, int min, int ave, int max);
    void speedUp();
    void slowDown();
    void stop();
    void gunIt();
    void go();
    void changeDirectionFwd();
    void changeDirectionRev();
    void resendSpeed();
    
    void setSpeed(int speed, bool dir);
    
    void interrupter(int int_code);
    
	void printInfo();
    
  private:
    Speeds speeds;
    
    int _cur_speed;
    int _state;
    Commander* cmdr;
    
    
    void _speedUp();
    void _slowDown();
    
};

#endif