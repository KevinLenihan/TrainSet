/*
  Engine.cpp - class for controlling engines
  Created By Kevin Lenihan
*/

#include "Arduino.h"
#include "Engine.h"


Engine::Engine(Commander* cmder, int id, int min, int ave, int max){
  // initialize Commander handler
  cmdr = cmder;
  
  // set initial check values
  _id = id;
  _dir = FWD;
  
  speeds.min = min;
  speeds.average = ave;
  speeds.max = max;
  
  _cur_speed = speeds.average;

  _state = GO; // neither speeding up or slowing down
}

void Engine::printInfo(){
  Serial.print(_cur_speed);
  //Serial.print("");
  if (_dir == FWD)
    Serial.print("FWD; ");
  else
    Serial.print("REV; ");
  //setSpeed(_cur_speed,_dir);
}

void Engine::speedUp(){
  if (_state != SPEED_UP){
    _state = SPEED_UP; // disable any call to slow down
    _speedUp();
  }
}

void Engine::slowDown(){
  if (_state != SLOW_DOWN){
    _state = SLOW_DOWN;  // disable any call to speed up
    _slowDown();
  }
}

// gradually speeds up to speeds.average
void Engine::_speedUp(){
  if (_state != SPEED_UP)
    return;
  if (_cur_speed == speeds.average){
    _state = GO;
    return;
  }
  switch (_cur_speed){
    case 0:
      _cur_speed = speeds.min;
    break;
    default:
      _cur_speed += 2;
      if (_cur_speed > speeds.average)
        _cur_speed = speeds.average;
  }
  cmdr->sendCommand(_id); // select loco
  cmdr->sendCommand(_cur_speed, _dir); // set velocity
  
  cmdr->mgr->addInterrupt(SPEED_DELAY, this, SPEED_UP, true);
}

// gradually slows down to speed 6
void Engine::_slowDown(){
  if (_state != SLOW_DOWN)
    return;
  if (_cur_speed == speeds.min){
    _state = GO;
    return;
  }
  switch (_cur_speed){
    case 0:
      _state = STOP;
      return;
    default:
      _cur_speed -= 2;
      if (_cur_speed < speeds.min)
        _cur_speed = speeds.min;
  }
  cmdr->sendCommand(_id); // select loco
  cmdr->sendCommand(_cur_speed, _dir); // set velocity
  
  cmdr->mgr->addInterrupt(SPEED_DELAY, this, SLOW_DOWN, true);
}

void Engine::gunIt(){
  _cur_speed = speeds.max;
  cmdr->sendCommand(_id); // select loco
  cmdr->sendCommand(_cur_speed, _dir); // set velocity
}

void Engine::stop(){
  _cur_speed = 0;
  cmdr->sendCommand(_id); // select loco
  cmdr->sendCommand(_cur_speed, _dir); // set velocity
}

void Engine::go(){
  _cur_speed = speeds.average;
  cmdr->sendCommand(_id); // select loco
  cmdr->sendCommand(_cur_speed, _dir); // set velocity
}

void Engine::changeDirectionFwd(){
  _dir = FWD;
  if (_cur_speed == 0)
    return;
  cmdr->sendCommand(_id); // select loco
  cmdr->sendCommand(_cur_speed, _dir); // set velocity
}

void Engine::changeDirectionRev(){
  _dir = REV;
  if (_cur_speed == 0)
    return;
  cmdr->sendCommand(_id); // select loco
  cmdr->sendCommand(_cur_speed, _dir); // set velocity
}

void Engine::setSpeed(int spd, bool dir){
  _cur_speed = spd;
  _dir = dir;
  cmdr->sendCommand(_id); // select loco
  cmdr->sendCommand(_cur_speed, _dir); // set velocity
}

void Engine::resendSpeed(){
  cmdr->sendCommand(_id); // select loco
  cmdr->sendCommand(_cur_speed, _dir); // set velocity
}

void Engine::interrupter(int int_code){
  switch(int_code){
    case SPEED_UP:
      _speedUp();
    break;
    case SLOW_DOWN:
      _slowDown();
    break;
  }
}
