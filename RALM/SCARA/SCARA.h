#pragma once
#include "Arduino.h"
#ifndef SCARA_H
#define SCARA_H

class SCARA{
  private:
    int X,Y,Z;
    byte PUL,DIR,EN;
    long theta1,theta2;
    long last_step,start_step;
    unsigned long cur,pre = 0;
    long previous_angle = 0;
    long previous_step = 0;
    
    
  public:
    SCARA(byte pin,byte dir,byte EN);//생성자
    void ENABLE(bool state);//Enable 핀을 직접 제어가 가능한 함수
    long Theta1(long x,long y);//세타 1을 구하는 메소드
    long Theta2(long x,long y);//세타 2를 구하는 메소드
    void SetStep(long step);//스텝 설정
    bool Whether();//남은 스텝 여부
    void Move(unsigned long duration);//스텝모터 구동
    long preAngle(long angle);//상대각도
    long preStep(long step);//이전 스텝을 저장하는 함수
    void show();//원하는 정보를 보고싶을때 
    long T_mm2step(long mm);//Top 부분의 mm를 스텝으로 변경하는 함수
    long M_angle2step(long angle);//Middle 부분의 각도를 스텝으로 변경하는 함수
    long B_angle2step(long angle);
    int H_angle2step(int angle); 
    void test(unsigned long cnt);//스텝모터 테스트 메소드
};
#endif
