/*
  Command.h - defines command format to allow other entities 
    to send commands through this class 
  Created By Kevin Lenihan
*/
#ifndef Command_h
#define Command_h

#include "Arduino.h"

#define BOARD 8
#define DECODER 9

class DecoderID{
  public:
    int decoder;
    int board;
    int value;
    
    DecoderID(int _board, int _decoder);
};

class Command{
  public:
    Command(int board, int decoder, int value);
    Command(DecoderID* decoderID, int pin);
    void sendCommand();
    void sendCommand(int val);
    void clearLatch();
    int listen();
    
  private:
    DecoderID* _decoderID;
    int _board, _decoder, _value, _pin;
    
    void _enter(int type, int id);
};

// a fake command to use as a Null
class NoneCommand : public Command{
  public:
    NoneCommand();
    void sendCommand();
    void sendCommand(int val);
};

#endif