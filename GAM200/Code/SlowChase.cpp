///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SlowChase.cpp
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
#include "SlowChase.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
SlowChase::SlowChase(std::string name, float radius)
{
  Name(name);
  AggroRadius(radius);
  Timer& tim = Tim();
  tim.MaxTime(5.0f);
  //tim.Beep(true);
  Start();
}

SlowChase::SlowChase(SlowChase& ogSlowChase)
{
  Name(ogSlowChase.Name());
  speed = ogSlowChase.speed;
  AggroRadius(ogSlowChase.AggroRadius());
  Timer& tim1 = Tim();
  Timer& tim2 = ogSlowChase.Tim();
  tim1.MaxTime(tim2.MaxTime());
  Start();
}

void SlowChase::Speed(float val)
{
  speed = val;
}
