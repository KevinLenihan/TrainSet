/*
  Light.h - defines a single Light
  Created By Kevin Lenihan
*/
#ifndef Light_h
#define Light_h

#include "Arduino.h"
#include "Command.h"

enum light_t {GREEN, YELLOW, RED, OUT};

class Light{
  public:
    Command* green;
    Command* yellow;
    Command* red;
    int color;
    
    virtual void setGreen() = 0;
    virtual void setYellow() = 0;
    virtual void setRed() = 0;
    virtual void setClear() = 0;
    
    int getLight();
};

class BiStateLight : public Light{
  public:
    BiStateLight(Command* _green, Command* _red);
    void setGreen();
    void setYellow();
    void setRed();
    void setClear();
};

class TriStateLight : public Light{
  public:
    TriStateLight(Command* _green, Command* _yellow, Command* _red);
    void setGreen();
    void setYellow();
    void setRed();
    void setClear();
};

#endif