///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: BehaviorScripts.cpp
//
//  Author: Sonya Toft
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "BehaviorScripts.h"
#include "ScreenManager.h"
#include "Engine.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
//float BehaviorScript::Timer()
//{
//  return timer_;
//}
//
//void BehaviorScript::Timer(float time)
//{
//  timer_ = time;
//}
//
//void BehaviorScript::AdjustTimer(float amount)
//{
//  timer_ += amount;
//
//  /*if (timer_ <= 0.0f || timer_ > timerMax_)
//    timer_ = timerMax_;*/
//}
//
//float BehaviorScript::TimerMax()
//{
//  return timerMax_;
//}
//
//void BehaviorScript::TimerMax(float newMax)
//{
//  timerMax_ = newMax;
//}
//
//void BehaviorScript::ResetTimer()
//{
//  timer_ = timerMax_;
//}

Timer & BehaviorScript::Tim()
{
  return tim_;
}

string BehaviorScript::Name()
{
  return name_;
}

void BehaviorScript::Name(string name)
{
  name_ = name;
}

float BehaviorScript::AggroRadius()
{
  return aggroRadius_;
}

void BehaviorScript::AggroRadius(float radius)
{
  aggroRadius_ = radius;
}

bool BehaviorScript::IsAggroed()
{
  return isAggroed_;
}

void BehaviorScript::IsAggroed(bool& aggro)
{
  isAggroed_ = aggro;

  if (aggro == true)
  {
    static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->AddAggro();
  }
  
}
