/*
  Command.cpp - defines all command functionality
  Created By Kevin Lenihan
*/

#include "Command.h"

DecoderID::DecoderID(int _board, int _decoder){
  board = _board;
  decoder = _decoder;
  value = 0;
}

Command::Command(int board, int decoder, int value){
  _board = board;
  _decoder = decoder;
  _value = value;
  _decoderID = NULL;
}

Command::Command(DecoderID* decoderID, int pin){
  _decoderID = decoderID;
  _board = _decoderID->board;
  _decoder = _decoderID->decoder;
  _pin = pin;
}

void Command::sendCommand(){
  // select board
  _enter(BOARD, _board);
  
  // write value to bus
  digitalWrite(10, bitRead(_value,0));
  digitalWrite(11, bitRead(_value,1));
  digitalWrite(12, bitRead(_value,2));
  digitalWrite(13, bitRead(_value,3));
  
  // enabe latch for data
  _enter(DECODER, _decoder);
  
  // disable latch, preserves data
  _enter(DECODER, 0);
}

void Command::sendCommand(int val){
  if (_decoderID == NULL) return;
  
  bitWrite(_decoderID->value, _pin, val);
  _value = _decoderID->value;
  
  sendCommand();
}

void Command::clearLatch(){
  _enter(BOARD, _board);

  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  
  _enter(DECODER, _decoder);
  
  _enter(DECODER, 0);

}

void Command::_enter(int type, int id){
  // type: either BOARD or DECODER def
  // num: board or decoder id
  digitalWrite(4, bitRead(id,0));
  digitalWrite(5, bitRead(id,1));
  digitalWrite(6, bitRead(id,2));
  digitalWrite(7, bitRead(id,3));
  digitalWrite(type, HIGH);
  digitalWrite(type,LOW);
}

NoneCommand::NoneCommand() : Command(0,0,0){}
void NoneCommand::sendCommand(){}
void NoneCommand::sendCommand(int val){}

