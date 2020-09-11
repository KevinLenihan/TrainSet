/*
  Rail.h - defines a single rail of the trainSet. 
           Used for disabling detectors to debounce the signal
  Created By Kevin Lenihan
*/
#ifndef Rail_h
#define Rail_h

#include "Arduino.h"
#include "Detector.h"

#define MAX_NUM_RAILS 8

class Rail{
  public:
    Rail();
    void addDetector(Detector* det);
    void debounce();
    
  private:
    Detector* detectors[NUM_DETECTORS];
    int size;
};

class RailList{
  public:
    RailList();
    void addRail(Rail* r);
    void debounce();
    
  private:
    Rail* rails[MAX_NUM_RAILS];
    int size;
};

#endif
