/*
  Crossing.cpp - defines scope of a crossing and
           controls traffic lights for TrainSet
  Created By Kevin Lenihan
*/

#include "Arduino.h"
#include "Crossing.h"



Crossing::Crossing(Manager* man, Rail* rail, CrossingLight* xing, Engine* e,
  Detector* app, Detector* _at, Detector* clr){
  light = xing;
  mgr = man;
  eng = e;
  num_delay_ints = 0;
  
  if (light != NULL)
    light_dir = light->addCrossing();
  approach = app;
  at = _at;
  clear = clr; 
  
  rail->addDetector(approach);
  rail->addDetector(at);
  rail->addDetector(clear);
  
}

int Crossing::getLight(){
  if (light != NULL)
    return light->getState(light_dir);
  return -1;
}

void Crossing::approachInt(int edge_trigger, int delay, int funct){
  mgr->addInterrupt(&approach, edge_trigger, delay, this, funct, false);
}

void Crossing::atInt(int edge_trigger, int delay, int funct){
  mgr->addInterrupt(&at, edge_trigger, delay, this, funct, false);
}

void Crossing::clearInt(int edge_trigger, int delay, int funct){
  mgr->addInterrupt(&clear, edge_trigger, delay, this, funct, false);
}

// swaps (at and clear)
void Crossing::swap(){
  Detector* temp;
  temp = at;
  at = clear;
  clear = temp;
}

void Crossing::addIntersection(Crossing* c){intersection = c;}

void Crossing::addLight(int _light_dir){light_dir = _light_dir;}

void Crossing::interrupter(int int_code){
  switch (int_code){
    case STOP:
      eng->stop();
      break;
    case GO:
      eng->go();
      break;
    case CHANGE_DIRECTION_FWD:
      if (intersection != NULL && eng->_dir != FWD)
        intersection->swap();
      eng->changeDirectionFwd();
      break;
    case CHANGE_DIRECTION_REV:
      if (intersection != NULL && eng->_dir != REV)
        intersection->swap();
      eng->changeDirectionRev();
      break;
    case SPEED_UP:
      eng->speedUp();
      break;
    case SLOW_DOWN:
      eng->slowDown();
      break;
    case SLOW_DOWN_INTERSECTION: // If light turns green then speed up. Otherwise slow down and check later
      switch (getLight()){
        case GREEN:
          eng->speedUp();
          break;
        case RED:
          eng->slowDown();
          mgr->addInterrupt(ENGINE_RESPONSE_TIME, this, SLOW_DOWN_INTERSECTION, true);
          break;
      }
      break;
    case LIGHT_GREEN:
      if (light != NULL)
        light->setState(light_dir, GREEN);
      break;
    case LIGHT_RED:
      if (light != NULL)
        light->setState(light_dir, RED);
      break;
    case RANDOM_STOP:
      if (random(2)){ // random 50/50 chance
        eng->stop();
        // create random delay between the min and max
        mgr->addInterrupt((MIN_RANDOM_DELAY + 
          (random(MAX_RANDOM_DELAY - MIN_RANDOM_DELAY))),
          this, GO, true);
      }
      break;
    case OBEY_LIGHT:
      switch (getLight()){
        case GREEN:
          eng->go();
          break;
        case YELLOW:
          eng->stop();
          mgr->addInterrupt(ENGINE_RESPONSE_TIME,this, WAITING,true); // delay of 0
          break;
        case RED:
          eng->stop();
          mgr->addInterrupt(ENGINE_RESPONSE_TIME,this, WAITING,true); // delay of 0
          break;
      }
      break;
    case OBEY_LIGHT_PHILLY:
      switch (getLight()){
        case GREEN:
          eng->go();
          break;
        case YELLOW:
          eng->gunIt();
          break;
        case RED:
          eng->stop();
          mgr->addInterrupt(ENGINE_RESPONSE_TIME,this, WAITING,true); // delay of 0
          break;
      }
      break;
    case WAITING:
      if (getLight() == GREEN)
        eng->go();
      else
        mgr->addInterrupt(ENGINE_RESPONSE_TIME,this, WAITING,true); // wait again
      break;
    case FIRST:
      if (light != NULL){
        light->setFirst(light_dir);
        if (getLight() == RED){ // is second there
          mgr->addInterrupt(ENGINE_RESPONSE_TIME, this, SLOW_DOWN_INTERSECTION, true);
        }
      }
      break;
    case FIRST_CLEAR:
      if (light != NULL)
        light->clearFirst(light_dir);
      break;
  }
}

//Crossings without yellow lights
CrossingLight::CrossingLight(Manager* man, Command* ag, 
    Command* ar, Command* bg, Command* br){
  // Initialize Crossing
  _crossing_count = 0;
  num_states = 2;
  
  light[0] = new BiStateLight(ag,ar);
  light[1] = new BiStateLight(bg,br);
  mgr = man;
  _cur = 0;
  
  enable_interrupt = true;
  first = -1;
  second = -1;
  
  states = (State*)malloc(sizeof(State) * num_states);
  states[0].color[0] = GREEN;
  states[0].color[1] = RED;
  states[0].dur = SEC;
  
  states[1].color[0] = RED;
  states[1].color[1] = GREEN;
  states[1].dur = SEC;
  
  setState(0, RED);
}

CrossingLight::CrossingLight(Manager* man, Command* ag, Command* ay, 
    Command* ar, Command* bg, Command* by, Command* br){
  int temp;
  
  _crossing_count = 0;
  num_states = 6;
  enable_interrupt = true;
  first = -1;
  second = -1;
  
  light[0] = new TriStateLight(ag, ay, ar);
  light[1] = new TriStateLight(bg, by, br);
  
  states = (State*)malloc(sizeof(State) * num_states);
  
  states[0].color[0] = GREEN;
  states[0].color[1] = RED;
  states[0].dur = GREEN_TIME * SEC;
  
  states[1].color[0] = YELLOW;
  states[1].color[1] = RED;
  states[1].dur = YELLOW_TIME * SEC;
  
  states[2].color[0] = RED;
  states[2].color[1] = RED;
  states[2].dur = RED_TIME * SEC;
  
  states[3].color[0] = RED;
  states[3].color[1] = GREEN;
  states[3].dur = GREEN_TIME * SEC;
  
  states[4].color[0] = RED;
  states[4].color[1] = YELLOW;
  states[4].dur = YELLOW_TIME * SEC;
  
  states[5].color[0] = RED;
  states[5].color[1] = RED;
  states[5].dur = RED_TIME * SEC;
  
  mgr = man;
  
  _cur = 0;
  
  light[0]->setGreen();
  light[1]->setRed();
  temp = (int)states[_cur].dur;
  mgr->addInterrupt(temp, this, 0, true);
  
}

void CrossingLight::printInfo(){
  
}

int CrossingLight::addCrossing(){return _crossing_count++;}

int CrossingLight::getState(int dir){return light[dir]->getLight();}

void CrossingLight::setState(int dir, int color){
  switch (color){
    case GREEN:
      light[dir]->setGreen();
      light[(dir+1)%2]->setRed(); // set other red
    break;
    case RED:
      light[dir]->setRed();
      light[(dir+1)%2]->setGreen(); // set other green
    break;
  }
}

void CrossingLight::setFirst(int dir){
  if (first == -1){
    first = dir;
    setState(first, GREEN);
  }
  else
    second = dir;
}

void CrossingLight::clearFirst(int dir){
  if (first == dir)
    first = -1;
  else
    second = -1;
  setState(dir, RED);
}

void CrossingLight::disableInterrupts(){enable_interrupt = false;}

void CrossingLight::interrupter(int int_code){
  if (!enable_interrupt)
    return;
  switch (num_states){
    case 6:
      _changeTristate();
    break;
    case 2:
      _changeBistate();
    break;
  }
}

void CrossingLight::_changeTristate(){
  _cur = (_cur+1) % num_states; // update _cur to next in the array
  
  switch (states[_cur].color[0]){ // send new color cmd to N/S light
    case GREEN:
      //Serial.print("Setting N/S light to Green\n");
      light[0]->setGreen();
    break;
    case YELLOW:
      //Serial.print("Setting N/S light to Yellow\n");
      light[0]->setYellow();
    break;
    case RED:
      //Serial.print("Setting N/S light to Red\n");
      light[0]->setRed();
    break;
  } 
  
  switch (states[_cur].color[1]){ // send new color cmd to E/W light
    case GREEN:
      //Serial.print("Setting E/W light to Green\n");
      light[1]->setGreen();
    break;
    case YELLOW:
      //Serial.print("Setting E/W light to Yellow\n");
      light[1]->setYellow();
    break;
    case RED:
      //Serial.print("Setting E/W light to Red\n");
      light[1]->setRed();
    break;
  }
  

  //Serial.print("Duration: ");
  //Serial.print(states[_cur].dur);
  //Serial.print("\n\n");
  
  mgr->addInterrupt(states[_cur].dur, this, 0, true);
}

void CrossingLight::_changeBistate(){
  _cur = (_cur+1) % num_states; // update _cur to next in the array
  
  switch (states[_cur].color[0]){ // send new color cmd to N/S light
    case GREEN:
      Serial.print("Setting N/S light to Green\n");
      light[0]->setGreen();
    break;
    case RED:
      Serial.print("Setting N/S light to Red\n");
      light[0]->setRed();
    break;
  } 
  
  switch (states[_cur].color[1]){ // send new color cmd to E/W light
    case GREEN:
      //Serial.print("Setting E/W light to Green\n");
      light[1]->setGreen();
    break;
    case RED:
      //Serial.print("Setting E/W light to Red\n");
      light[1]->setRed();
    break;
  }  
  mgr->addInterrupt(states[_cur].dur, this, 0, true);
}

