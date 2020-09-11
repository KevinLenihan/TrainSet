/*
  Detector.cpp - detector functionality
  Created By Kevin Lenihan
*/

#include "Arduino.h"
#include "Detector.h"
#include "Command.h"

NoneDetector::NoneDetector(){}

int NoneDetector::test(){return 0;}

void NoneDetector::enable(bool _enable){}

Detector::Detector(){
  cur = 0;
  prev = 0;
  flag = NO_EDGE;
  enabled = true;
}

void Detector::enable(bool _enable){enabled = _enable;}

// returns either, NO_EDGE, RISING_EDGE, or FALLING_EDGE
int Detector::test(){
  if (!enabled)
    return NO_EDGE;
  return flag;
}

void Detector::update(int value){
  prev = cur;
  cur = value;
  if (prev == cur){
    flag = NO_EDGE;
    return;
  }
  if (cur == 1)
    flag = RISING_EDGE;
  else
    flag = FALLING_EDGE;
}

DetectorManager::DetectorManager(){
  detectors = (Detector*)malloc(sizeof(Detector)*NUM_DETECTORS);
  for (int i=0;i<NUM_DETECTORS;i++){
    detectors[i] = Detector();
  }
}

// reads all the detectors at once
void DetectorManager::detect(){
  //Set bus to input
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);
  
  _enter(BOARD, DETECT_BOARD);
  
  //read in detectors
  for (int decoder = 1, i = 0; decoder <= (NUM_DETECTORS +3)/4;decoder++){
    _enter(DECODER, decoder);
    
    //update detector list
    detectors[i++].update(digitalRead(10));
    detectors[i++].update(digitalRead(11));
    detectors[i++].update(digitalRead(12));
    detectors[i++].update(digitalRead(13));
  }
  _enter(DECODER, 0);
  // Reset bus to output
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
}

void DetectorManager::_enter(int type, int id){
  /*
  Serial.print("ent(t:"); Serial.print(type); Serial.print(" id:");
  Serial.print(id); Serial.print(")\n");
  */
  // type: either BOARD or DECODER def
  // num: board or decoder id
  digitalWrite(4, bitRead(id,0));
  digitalWrite(5, bitRead(id,1));
  digitalWrite(6, bitRead(id,2));
  digitalWrite(7, bitRead(id,3));
  digitalWrite(type, HIGH);
  digitalWrite(type,LOW);
}