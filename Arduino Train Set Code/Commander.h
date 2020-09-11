/*
  Commander.h - Handles DCC command queue so commands 
    have enough time to be sent, 12 millis seconds.
  Created By Kevin Lenihan
*/
#ifndef Commander_h
#define Commander_h

#include "Arduino.h"
#include "Command.h"
#include "Entity.h"
#include "Manager.h"

#define FWD 0
#define REV 1

#define QUEUE_SIZE 32
#define DCC_DELAY 200 // milliseconds

class Commander : public Entity{
  public:
    Manager* mgr; // handler to Manager
  
    Commander(Manager* man);
    int sendCommand(int spd, int dir); // speed command format
    int sendCommand(int id); // select command format
    
    void interrupter(int int_code); // activated when queue is not empty
    bool is_empty(); // returns true if 
    
  private:
    
    int _DCC_queue[QUEUE_SIZE]; // DCC command index queue used to add 12 millisecond delay 
                                // between commands
    unsigned long _last_command_time;
    int _prev_cmd; // used to clear latch for next command
    int _index;
    int _size;
    int _engine_list_size;
    
    int _getSpeedCommand(int spd, int dir); // return speed command index
    int _getSelectCommand(int id); // return select locomotive command index
    void _refresh_engine_speed();
};

#endif