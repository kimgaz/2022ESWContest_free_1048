#include "SCARA.h"
#include "Arduino.h"
#define S 143L //outside arm length,(unit:mm) 
#define M 238L //inner arm length,(unit:mm)

SCARA::SCARA(byte pin,byte dir,byte en){
  /* AVR PORT PIN MAP
     *  ------------ DIR,PULSE
     *  PORT A0 - A7
     *  DPIN 22 - 29
     *  ------------ ENABLE
     *  PORT C0 - C7
     *  DPIN 37 - 30
  */

  /* Coordinate rate
    *
    * Coordinate = degree : step
    * 1/2 step
    * Bottom = 90 : 2000(5 rotation)
    *
    * Coordinate = degree : step
    * 1/8 step
    * Middle = 90 : 12800(4 rotation)
    *
    * Coordinate = degree : mm
    * 1/8 step
    * Top = 360 : 8()
    *
    * Coordinate = degree : step
    * 1/8 step
    * Head = 360 : 7200
    * Head = 1 : 20
    *
    *
    * 가동범위
    * Z -247 ~ 0mm
    *
  */

  pinMode(pin,OUTPUT);
  pinMode(dir,OUTPUT);
  pinMode(en,OUTPUT);

  this->PUL = 1<<(pin-22);
  this->DIR = 1<<(dir-22);
  this->EN = 1<<(37-en);

  DDRA = 0xFF;
  DDRC = 0xFF;
  PORTC = 0xFF;
}

void SCARA::ENABLE(bool state){
  if(state == HIGH){
    PORTC = PINC | this->EN; //EN HIGH
  }
  else{
    PORTC = PINC & ~this->EN; //EN LOW
  }
}

long SCARA::Theta1(long x,long y){

  long var1 = sq(M)+sq(S)-(sq(x)+sq(y));
  long var2 = 2*M*S;
  double val = (double)var1/(double)var2;
  long the1 = round(180/PI*acos(val));
  //Serial.print("the1:");
  //Serial.println(the1);
  the1 = 180 - the1;

  if(x >= 0){//1,4분면
    if(y >= 0){//1사분면
      this->theta1 = -the1;    
    }
    else{//4사분면
      this->theta1 = -the1;
    }
  }
  else{//2,3사분면
    if(y >= 0){//2사분면
      this->theta1 = the1;
    }
    else{//3사분면
      this->theta1 = the1;
    }
  }
  return this->theta1;
}

long SCARA::Theta2(long x,long y){
  long var1 = sq(M) + sq(x) + sq(y) - sq(S);
  double var2 = 2*M*(double)sqrt(sq(x)+sq(y));
  double val = (double)var1/(double)var2;
  long the2 = round(180/PI*acos(val));
  //Serial.print("the2:");
  //Serial.println(the2);
  double all = round(180/PI*atan2(y,x));

  if(x>=0){//1,4사분면
    if(x == 381){
      this->theta2 = 90;
    }
    else{
      if(y>=0){//1사분면
        long gap = all - the2;
        this->theta2 = 90 - gap;
      }
      else{//4사분면
        this->theta2 = the2;
      }
    }
    
  }
  else{//2,3사분면
    if(x == -381){
      this->theta2 = -90;
    }
    else{
      if(y>=0){//2사분면
        the2 = the2 + all;
        this->theta2 = -the2 + 90;
      }
      else{//3사분면

        this->theta2 = -the2;
      }
    }
  }
  return this->theta2;
}

void SCARA::SetStep(long step){
  PORTC = PINC & ~this->EN; //EN LOW
  if(step >= 0){ //스텝이 양수면
    PORTA = PINA | this->DIR;//DIR set HIGH
    this->start_step = step;
    this->last_step = this->start_step;
  }
  else{//스텝이 음수면
    PORTA = PINA & ~this->DIR;//DIR set LOW
    this->start_step = -step;
    this->last_step = this->start_step;
  }
}

bool SCARA::Whether(){
  if(this->last_step <= 0){
    PORTC = PINC | this->EN; //EN HIGH
    return 0;
  }
  else{
    return 1;
  }
}

void SCARA::Move(unsigned long duration){
  if(this->last_step > 0){//if last_step is exist
    this->cur = micros();
    if(this->cur - this->pre >= duration){
      this->pre = this->cur;
      if(PINA & this->PUL){//if PULSE PIN HIGH
        PORTA = PINA & ~this->PUL;
      }
      else{//if PULSE PIN LOW
        PORTA = PINA | this->PUL;
        this->last_step--;
      }
    }
  }
}

long SCARA::preAngle(long angle){
  long last = angle - this->previous_angle;
  this->previous_angle = angle;
  return last;
}

long SCARA::preStep(long step){
  float last = step - this->previous_step;
  this->previous_step = step;
  return last;
}

void SCARA::show(){
  Serial.print("LAST_STEP:");
  Serial.println(this->last_step);
  Serial.print("PUL:");
  Serial.println(this->PUL,HEX);
  Serial.print("EN:");
  Serial.println(this->EN);
}

long SCARA::T_mm2step(long mm){
  return mm*228;
}

long SCARA::M_angle2step(long angle){
  return round(angle*142);
}

long SCARA::B_angle2step(long angle){
  return round(angle*178);
}

int SCARA::H_angle2step(int angle){
  return angle*20;
}

void SCARA::test(unsigned long cnt){
  for(unsigned long cnt1 = 0;cnt1 <= cnt; cnt1++){
    PORTC = 0x00;
    PORTA = 0x02;
    delay(1000);
    PORTA = 0x03;
    delay(1000);
  }
}
