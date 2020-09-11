/*
  Crossing.h - defines scope of a railway and
           controls traffic lights for TrainSet
  Created By Kevin Lenihan
*/
#ifndef Crossing_h
#define Crossing_h

#include "Arduino.h"
#include "Command.h"
#include "Manager.h"
#include "Entity.h"
#include "Detector.h"
#include "Rail.h"
#include "Light.h"
#include "Engine.h"

#define GREEN_TIME 11
#define YELLOW_TIME 3
#define RED_TIME 1 // double red time
#define SEC 1000

#define MIN_RANDOM_DELAY 2000
#define MAX_RANDOM_DELAY 12000
#define ENGINE_RESPONSE_TIME 250
#define MAX_NUM_DELAY_INTS 5

typedef struct State{
  int color [2];
  int dur;
};

class CrossingLight : public Entity{
  public:
    CrossingLight(Manager* man, 
      Command* ag, Command* ar, Command* bg, Command* br); // for Crossings without yellow lights
    CrossingLight(Manager* man,
      Command* ag, Command* ay, Command* ar, Command* bg, Command* by, Command* br);
      // for crossings with yellow lights 
    int getState(int dir); // returns a light_t
    void setState(int dir, int color);
    
    int addCrossing();
    void disableInterrupts();
    
    void setFirst(int dir);
    void clearFirst(int dir);
    
    void printInfo();
    
    void interrupter(int int_code);
    
  private:
    Manager* mgr;
    Light* light [2];
    int _cur;
    bool enable_interrupt;
    State* states;
    int num_states; // also used to define type of crossing
    int _crossing_count;
    
    int first, second; // -1 used for null
    
    void _changeTristate();
    void _changeBistate();
};

class Crossing : public Entity{
  public:
    int light_dir;
    
    Crossing(Manager* man, Rail* rail, CrossingLight* xing, Engine* e,
      Detector* app, Detector* _at, Detector* clr);
    int getLight();
    
    void approachInt(int edge_trigger, int delay, int funct);
    void atInt(int edge_trigger, int delay, int funct);
    void clearInt(int edge_trigger, int delay, int funct);

    void addLight(int light_dir);
    
    void addIntersection(Crossing* c);
    void swap();
    
    
    void interrupter(int int_code);
    
  private:
    Manager* mgr;
    CrossingLight* light;
    Engine* eng;
    
    int delay [MAX_NUM_DELAY_INTS];
    int d_funct [MAX_NUM_DELAY_INTS];
	int num_delay_ints;
    Detector* approach;
    Detector* at;
    Detector* clear;
    
    Crossing* intersection;

};

#endif