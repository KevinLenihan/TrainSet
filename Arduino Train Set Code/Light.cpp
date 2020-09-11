/*
  Light.cpp - defines a single Light
  Created By Kevin Lenihan
*/
#include "Arduino.h"
#include "Light.h"

int Light::getLight(){
  return color;
}

BiStateLight::BiStateLight(Command* _green, Command* _red){
  green = _green;
  red = _red;
}

void BiStateLight::setGreen(){
  green->sendCommand(1);
  red->sendCommand(0);
  color = GREEN;
}

void BiStateLight::setRed(){
  green->sendCommand(0);
  red->sendCommand(1);
  color = RED;
}

void BiStateLight::setClear(){
  green->sendCommand(0);
  red->sendCommand(0);
  color = OUT;
}

void BiStateLight::setYellow(){} // stub

TriStateLight::TriStateLight(Command* _green, Command* _yellow, Command* _red){
  green = _green;
  yellow = _yellow;
  red = _red;
}

void TriStateLight::setGreen(){
  green->sendCommand();
  color = GREEN;
}

void TriStateLight::setYellow(){
  yellow->sendCommand();
  color = YELLOW;
}

void TriStateLight::setRed(){
  red->sendCommand();
  color = RED;
}

void TriStateLight::setClear(){}
