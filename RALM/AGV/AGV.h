#pragma once
#include "Arduino.h"
#ifndef AGV_H
#define AGV_H

class AGV{
private:
    byte PUL_R,DIR_R,EN_R;
    byte PUL_L,DIR_L,EN_L;
    byte A,B;
    bool state = LOW;
    unsigned long steps = 0;

public:
    AGV(byte a,byte b);
    void setLeft(byte pulse,byte dir,byte en);
    void setRight(byte pulse,byte dir,byte en);
    void DIR(bool R,bool L);
    void Move(word interval);
    void setStep(unsigned long steps);
    bool still();
    void stepMoving(word interval);
    void justMove(word walk);
    int spotA();
    int spotB();
};
#endif