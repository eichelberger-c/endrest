///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: FastChase.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - FastChase
//      - FastChase (copy constructor)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "FastChase.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
FastChase::FastChase(std::string name, float radius)
{
  Name(name);
  AggroRadius(radius);
  Start();
}

FastChase::FastChase(FastChase& ogFastChase)
{
  Name(ogFastChase.Name());
  AggroRadius(ogFastChase.AggroRadius());
  Start();
}

void FastChase::ChaseSpeed(float val)
{
  chaseSpeed = val;
}

void FastChase::DashSpeed(float val)
{
  dashSpeed = val;
}

void FastChase::DashTimer(float val)
{
  timerDash = val;
}

void FastChase::PauseTimer(float val)
{
  timerPause = val;

  pauseLimit = timerDash - timerPause;
}

void FastChase::Range(float val)
{
  range = val;
}
