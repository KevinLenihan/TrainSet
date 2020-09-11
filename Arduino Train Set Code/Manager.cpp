/*
  Manager.h - manages the interrupts
  Created By Kevin Lenihan
*/
#include "Manager.h"

LinkedList::LinkedList(){
  head = NULL;
}

void LinkedList::add(Interrupt* i){ // adds an Interrupt to the end of the list
  Interrupt* temp = head;
  //Serial.print("Adding interrupt");
  
  if (temp == NULL){
    i->next = NULL;
    head = i;
    return;
  }
  while (temp->next != NULL){
    temp = temp->next;
  }
  temp->next = i;
  i->next = NULL; // probably don't need this line
}

void LinkedList::loop(){ // _tests every element in the list
  Interrupt* temp;
  Interrupt* prev;
  //int i = 0;
  
  prev = NULL;
  temp = head;
  while(temp!=NULL){
    if (_test(temp)){
      if (temp == head){
		    head = temp->next;
        free(temp);
        temp = head;
        continue;
      }
	    else{
        prev->next = temp->next;
        free(temp);
        temp = prev->next;
        continue;
	    }
    }
    prev = temp;
    temp = temp->next;
  }
}

int LinkedList::_test(Interrupt* i){ // returns true if interrupt should be removed
  int ret = 0;
  int delay = 0;
  
  if (i->type == DETECTOR){ // is detector based interrupt
    if ((*(i->detector_p))->test() == i->trigger){ // has detector triggered
	  if (i->dur != 0){	  // if there is a delayed reaction change interrupt to timed
        i->type = DELAYED_DETECTOR; // Reactionary interrupt
		i->start = millis();
		return 0; // now wait for delay to run out
      }
	  i->object->interrupter(i->int_code); // call function
	  ret = 1;
    }
  }
  else{ // has enough time elapsed
    delay = i->start + i->dur - millis();
    if (i->start + i->dur <= millis()){
      if (i->type == DELAYED_DETECTOR){
        i->type = DETECTOR;		
      }
      i->object->interrupter(i->int_code); // call function
      ret = 1;
    }
  }
  
  if (!i->perishable){
    if (i->detector_p == NULL && ret == 1) // timed non-perishable interrupt
      i->start = millis();
    return 0;
  }
  return ret;
}

/*
  Manager class
*/

Manager::Manager(DetectorManager* dm){
// empty constructor no need to initialize anything
// header file initializes LinkedList
  det_mgr = dm;
  rail_list = new RailList();
  for (int i = 0; i <= 7;i++){
  
  }
}

void Manager::addInterrupt(Detector* detector, int edge_trigger, int dur, Entity* object, int int_code, bool perishable){
  Interrupt* i;

  i = (Interrupt*)malloc(sizeof(Interrupt));
  i->type = DETECTOR;
  i->start = 0;
  i->dur = dur;
  i->det = detector;
  i->detector_p = &(i->det);
  i->trigger = edge_trigger;
  i->perishable = perishable;
  i->object = object;
  i->int_code = int_code;
  list.add(i);
}

void Manager::addInterrupt(Detector** detector, int edge_trigger, int dur, Entity* object, int int_code, bool perishable){
  Interrupt* i;
  i = (Interrupt*)malloc(sizeof(Interrupt));
  i->type = DETECTOR;
  i->start = 0;
  i->dur = dur;
  i->det = NULL;
  i->detector_p = detector;
  i->trigger = edge_trigger;
  i->perishable = perishable;
  i->object = object;
  i->int_code = int_code;
  list.add(i);
}

void Manager::addInterrupt(int dur, Entity* object, int int_code, bool perishable){
  Interrupt* i;
  i = (Interrupt*)malloc(sizeof(Interrupt));
  i->type = DELAYED;
  i->start = millis();
  i->dur = dur;
  i->det = NULL;
  i->detector_p = NULL;
  i->perishable = perishable;
  i->object = object;
  i->int_code = int_code;
  list.add(i);
}

void Manager::run(){
  det_mgr->detect();
  list.loop();
  rail_list->debounce();
}