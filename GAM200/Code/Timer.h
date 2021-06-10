///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Timer.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Timer
//      - Timer (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"
#include "Utils.h"

///----------------------------------GLOBALS------------------------------------
typedef enum TimerTypes
{
  CountUp_,
  CountDown_,

} tType;

///----------------------------------CLASSES------------------------------------

class Timer : public Component
{
public:
  Timer();
  Timer(secs max, tType type = CountDown_, secs resetVal = 0.0f);
  Timer(const Timer& ogTimer);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset();
  void Adjust(secs time);
  void Set(secs newTime);
  void Time(secs newTime);
  secs Time();
  void MaxTime(secs newMax);
  secs MaxTime();
  void Type(tType newType);
  tType Type();
  void ResetVal(secs newVal);
  secs ResetVal();
  void Beep(bool toggle);
  bool Beep();

private:
  secs time_;
  secs maxTime_;
  tType type_;
  secs resetVal_;
  bool beep_;
};

///---------------------------------FUNCTIONS-----------------------------------


