///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Timer.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Timer.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

Timer::Timer(secs max, tType type, secs resetVal) : Component(Timer_)
{
  maxTime_ = max;
  type_ = type;
  resetVal_ = resetVal;
  beep_ = false;

  if (type == CountUp_)
  {
    time_ = 0.0f;
  }
  else
    time_ = max;
}

Timer::Timer(const Timer & ogTimer) : Component(Timer_)
{
  time_ = ogTimer.time_;
  maxTime_ = ogTimer.maxTime_;
  type_ = ogTimer.type_;
  resetVal_ = ogTimer.resetVal_;
  beep_ = false;
}

Component * Timer::Clone() const
{
  Timer *clone = new Timer;
  if (clone == nullptr)
    return nullptr;

  *clone = *this;

  return clone;
}

void Timer::Update(float dt)
{
  if (!IsActive() || beep_)
    return;

  switch (type_)
  {
  case CountUp_:
    if(beep_ == false)
      time_ += dt;

    if (time_ >= resetVal_)
      beep_ = true;
    break;
  case CountDown_:
    if (beep_ == false)
      time_ -= dt;

    if (time_ <= resetVal_)
      beep_ = true;
    break;
  }
}

void Timer::Reset()
{
  beep_ = false;

  if (type_ == CountUp_)
    time_ = 0.0f;
  else
    time_ = maxTime_;
}

void Timer::Adjust(secs time)
{
  time_ += time;
}

void Timer::Set(secs newTime)
{
  time_ = newTime;
}

void Timer::Time(secs newTime)
{
  time_ = newTime;
}

secs Timer::Time()
{
  return time_;
}

void Timer::MaxTime(secs newMax)
{
  maxTime_ = newMax;
}

secs Timer::MaxTime()
{
  return maxTime_;
}

void Timer::Type(tType newType)
{
  type_ = newType;
}

tType Timer::Type()
{
  return type_;
}

void Timer::ResetVal(secs newVal)
{
  resetVal_ = newVal;
}

secs Timer::ResetVal()
{
  return resetVal_;
}

void Timer::Beep(bool toggle)
{
  beep_ = toggle;
}

bool Timer::Beep()
{
  return beep_;
}

Timer::Timer() : Component(Timer_)
{
  time_ = 1.0f;
  maxTime_ = 1.0f;
  resetVal_ = 0.0f;
  type_ = CountDown_;
  beep_ = false;
}
