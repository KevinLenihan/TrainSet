/*
  InterruptingCow.h - manages the interrupts
  Created By Kevin Lenihan
*/

#ifndef Manager_h
#define Manager_h

#include "Arduino.h"
#include "Command.h"
#include "Detector.h"
#include "Rail.h"
#include "Entity.h"

enum int_class_t {DETECTOR, DELAYED, DELAYED_DETECTOR};

typedef struct Interrupt{
  int_class_t type;
  Detector** detector_p; // pointer to pointer of detector
  Detector* det; // pointer to detector
  int trigger;
  long start; // start time
  int dur; // delay duration
  bool perishable; // if true, Interrupt will be called once then removed
                  // otherwise, Interrupt will always stay in the list
  int int_code; // value passed to interrupter
  Entity* object; // pointer for calling interrupter(int int_code)
  
  // LinkedList component
  Interrupt* next;
}; // interrupt is also an element in a linked list

class LinkedList{
  public:
    LinkedList();
    void add(Interrupt* i);
    void loop();
   
  private:
    Interrupt* head;
    int _test(Interrupt* i);
};

class Manager{
  public:
    RailList* rail_list;
    
    Manager(DetectorManager* dm);
    void addInterrupt(Detector* detector, int edge_trigger, int dur, Entity* object, int int_code, bool perishable);
    void addInterrupt(Detector** detector, int edge_trigger, int dur, Entity* object, int int_code, bool perishable);
      //when dectector is tripped, call given function
    void addInterrupt(int dur, Entity* object, int int_code, bool perishable);
      // after time duration has passed, call function
    void run(); // loop function
    
  private:
    LinkedList list;
    DetectorManager* det_mgr;
};

#endif