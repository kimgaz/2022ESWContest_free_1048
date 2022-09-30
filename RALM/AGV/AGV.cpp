#include "AGV.h"
#include "Arduino.h"

AGV::AGV(byte a,byte b){
    pinMode(a,INPUT);
    pinMode(b,INPUT);
    this->A = a;
    this->B = b;
}
void AGV::setRight(byte pulse,byte dir,byte en){
    this->PUL_R = pulse;
    this->DIR_R = dir;
    this->EN_R = en;

    pinMode(this->PUL_R,OUTPUT);
    pinMode(this->DIR_R,OUTPUT);
    pinMode(this->EN_R,OUTPUT);

    digitalWrite(this->EN_R,HIGH);
}
void AGV::setLeft(byte pulse,byte dir,byte en){
    this->PUL_L = pulse;
    this->DIR_L = dir;
    this->EN_L = en;

    pinMode(this->PUL_L,OUTPUT);
    pinMode(this->DIR_L,OUTPUT);
    pinMode(this->EN_L,OUTPUT);

    digitalWrite(this->EN_L,HIGH);
}
void AGV::DIR(bool R,bool L){
    if(R==1&&L==1){
        //go ahead
        digitalWrite(this->DIR_R,HIGH);
        digitalWrite(this->DIR_L,LOW);
    }   
    else if(R==0&&L==1){
        //turn Right
        digitalWrite(this->DIR_R,HIGH);
        digitalWrite(this->DIR_L,HIGH);
    }
    else if(R==1&&L==0){
        //turn Left
        digitalWrite(this->DIR_R,LOW);
        digitalWrite(this->DIR_L,LOW);
    }
    else if(R==0&&L==0){
        digitalWrite(this->DIR_R,LOW);
        digitalWrite(this->DIR_L,HIGH);
    }
}
void AGV::Move(word interval){
    static unsigned long cur_time,pre_time=0;
    cur_time = micros();
    if(cur_time - pre_time >= interval){
        pre_time = cur_time;
        if(this->state == LOW){
            this->state = HIGH;
        }
        else{
            this->state = LOW;
        }
    }
    digitalWrite(this->PUL_L,this->state);
    digitalWrite(this->PUL_R,this->state);
}
void AGV::setStep(unsigned long steps){
    this->steps = steps;
}
bool AGV::still(){
    if(this->steps > 0){

        return 1;
    }else{
        digitalWrite(this->EN_R,HIGH);
        digitalWrite(this->EN_L,HIGH);
        return 0;
    }
}
void AGV::stepMoving(word interval){
    static unsigned long cur,pre=0;
    if(this->steps > 0){
        cur = micros();
        if(cur - pre >= interval){
            pre = cur;
            if(this->state == LOW){
                this->state = HIGH;
            }
            else{
                this->state = LOW;
                this->steps--;
            }
        }
    }
    digitalWrite(this->PUL_R,this->state);
    digitalWrite(this->PUL_L,this->state);
}
void AGV::justMove(word walk){
    setStep(walk);
    while(still()){
        stepMoving(200);
    }
}
int AGV::spotA(){
    bool a = digitalRead(this->A);
    return a;
}
int AGV::spotB(){
    bool a = digitalRead(this->B);
    return a;
}