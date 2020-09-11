/*
  Rail.cpp - defines a single rail of the trainSet. 
           Used for disabling detectors to debounce the signal
  Created By Kevin Lenihan
*/
#include "Arduino.h"
#include "Rail.h"

Rail::Rail(){
  size = 0;
}

void Rail::addDetector(Detector* det){
  if (size < NUM_DETECTORS && det != NULL){
    detectors[size] = det;
    size++;
  }
}

void Rail::debounce(){
  bool tripped = false;
  
  // test if a detector has been tripped, RISING_EDGE only
  for (int i=0;i<size;i++){
    if (detectors[i]->test() == RISING_EDGE){
      tripped = true;
      break;
    }
  }
  if (!tripped) return;
  
  // enable all detectors that are not on a RISING EDGE
  // and disable the detector that is on its RISING EDGE
  for (int i=0;i<size;i++){
    if (detectors[i]->test() == RISING_EDGE)
      detectors[i]->enable(false);
    else
      detectors[i]->enable(true);
  }
}

RailList::RailList(){
  size = 0;
}

void RailList::addRail(Rail* r){
  if (size < MAX_NUM_RAILS){
    rails[size] = r;
    size++;
  }
}

void RailList::debounce(){
  for (int i=0;i<size;i++){
    rails[i]->debounce();
  }
}
