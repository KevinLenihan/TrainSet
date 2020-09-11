/*
  Detector.h - detector functionality
  Created By Kevin Lenihan
*/
#ifndef Detector_h
#define Detector_h

#include "Arduino.h"

#define DETECT_BOARD 3
#define NUM_DETECTORS 36

#define RISING_EDGE 1
#define FALLING_EDGE 2
#define NO_EDGE 0

class Detector{
  public:
    int cur; // value that the physical pin had, last time it was checked
  
    Detector();

    void enable(bool _enable);
    int test();
    void update(int value);
    
  private:
    
    int prev; //
    int flag;
    bool enabled;
};

// a fake detector to use as a Null
class NoneDetector: public Detector{
  public:
    NoneDetector();
    int test();
    void enable(bool _enable);
};

class DetectorManager{
  public:
    Detector* detectors;
    
    DetectorManager();
    void detect();
    
  private:
    void _enter(int type, int id);
};

#endif