/*
  Updater.cpp - Sends a periodic interrupt to resend engine speed commands
  Created By Kevin Lenihan
*/
#include "Arduino.h"
#include "Updater.h"

Updater::Updater(){
  size = 0;
  index = 0;
  
}

void Updater::update(){
  if ( size == 0 ){
    return;
  }
  index = (index + 1) % size;
  if (true) {
  engines[index]->resendSpeed();
  }
}

void Updater::addEngine(Engine* eng){
  if (size == MAX_NUM_RAILS){
    return;
  }
  engines[size] = eng;
  
}