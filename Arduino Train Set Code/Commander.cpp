/*
  Commander.cpp - Handles DCC command queue so commands 
    have enough time to be sent, 12 millis seconds.
  Created By Kevin Lenihan
*/

#include "Command.h"
#include "Commander.h"

// create the DCC_list
// valid values are 0-27
Command DCC_list [29] = {
  Command(4,1,8), // Stop FWD
  Command(4,1,4), // Stop REV
  Command(4,1,2), // Speed  4 FWD
  Command(4,1,1), // Speed  4 REV
  Command(4,2,8), // Speed  6 FWD
  Command(4,2,4), // Speed  6 REV
  Command(4,2,2), // Speed  8 FWD
  Command(4,2,1), // Speed  8 REV
  Command(4,3,8), // Speed 10 FWD
  Command(4,3,4), // Speed 10 REV
  Command(4,3,2), // Speed 12 FWD
  Command(4,3,1), // Speed 12 REV
  Command(4,4,8), // Speed 14 FWD
  Command(4,4,4), // Speed 14 REV
  Command(4,4,2), // Speed 16 FWD
  Command(4,4,1), // Speed 16 REV
  Command(4,5,8), // Speed 18 FWD
  Command(4,5,4), // Speed 18 REV
  Command(4,5,2), // Speed 20 FWD
  Command(4,5,1), // Speed 20 REV
  Command(4,6,8), // Speed 22 FWD
  Command(4,6,4), // Speed 22 REV
  Command(4,6,2), // Select 2014
  Command(4,6,1), // Select 2602
  Command(4,7,8), // Select 3030
  Command(4,7,4), // Select 3232
  Command(4,7,2), // Select 1009
  Command(4,7,1), // Select 1000
  Command(4,8,8)  // Select 1234
};

Commander::Commander(Manager* man){
  mgr = man;

  _index = 0;
  _size = 0;
  _last_command_time = 0;
  _prev_cmd = -1;

  
  _engine_list_size = 0;
  // initialize array elements to invalid
  for (int i=0;i<QUEUE_SIZE; i++){
    _DCC_queue[i] = -1; 
  }
  mgr->addInterrupt(DCC_DELAY, this, 0, false);
}

// adds a speed command to the queue
int Commander::sendCommand(int spd, int dir){
  // get cmd index
  int cmd = _getSpeedCommand(spd,dir);
  int end = (_index + _size) % QUEUE_SIZE; // get to end of list
  if (_size > QUEUE_SIZE)
    return -1;  //full queue, should never happen
    
  _DCC_queue[end] = cmd;
  _size++;
  //if (_size == 1) // if this is the first element in the queue
    //mgr->addInterrupt(DCC_DELAY, this, 0, true); // add interrupt with a delay
  return 0;
}

// adds a select command to the queue
int Commander::sendCommand(int id){
  // get cmd index
  int cmd = _getSelectCommand(id);
  int end = (_index + _size) % QUEUE_SIZE; // get to end of list
  if (_size > QUEUE_SIZE || cmd < 0)
    return -1;  //full queue or cmd returned and error, should never happen
    
  _DCC_queue[end] = cmd;
  _size++;
  
  //if (_size == 1) // if this is the first element in the queue
    //mgr->addInterrupt(DCC_DELAY, this, 0, true); // add interrupt with a delay
  return 0;
}

// call this interrupter every 12 millis seconds so long as the list is not empty
void Commander::interrupter(int int_code){ //originally named callNextCommand
  int cmd = _DCC_queue[_index];
  if (cmd == -1) // empty list
    return;
    
  if (_prev_cmd != -1) // prev command needs to be cleared before new command is added
    DCC_list[_prev_cmd].clearLatch();

  DCC_list[cmd].sendCommand();
  // update array data
  _prev_cmd = cmd; // update prev_cmd
  _DCC_queue[_index] = -1; // set used cmd value to invalid
  _index = (_index+1) % QUEUE_SIZE; // update index
  _size--;
  
  
  //if (_size > 0){
    //mgr->addInterrupt(DCC_DELAY, this, 0, true); // call this method again after delay
  //}  
}

// speed: actual speed value
// dir: FWD = 0; REV = 1
int Commander::_getSpeedCommand(int spd, int dir){
  int cmd = -1;
  switch (spd){
  case 0:
    cmd = 0; break;
  case 4:
    cmd = 1; break;
  case 6:
    cmd = 2; break;
  case 8:
    cmd = 3; break;
  case 10:
    cmd = 4; break;
  case 12:
    cmd = 5; break;
  case 14:
    cmd = 6; break;
  case 16:
    cmd = 7; break;
  case 18:
    cmd = 8; break;
  case 20:
    cmd = 9; break;
  case 22:
    cmd = 10; break;
  case 23:
    cmd = 11; break;
  default:
    return -1; // ERROR
  }
  
  cmd = cmd*2;
  if (dir == REV)
     cmd++;
  return cmd;
}

/*
    id | Train #
	0  | 2014
	1  | 2602
    2  | 3030
    3  | 3232
    4  | 1009
    5  | 1000
*/
int Commander::_getSelectCommand(int id){
  return id + 22;
}