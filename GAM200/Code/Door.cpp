///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Door.cpp
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Door
//      - Door (copy constructor)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Door.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: door constructor
Door::Door(std::string name, dState state, std::string area)
{
  Name(name);
  startState_ = state;
  Start();
  area = area;
  ID = "TEST";
  wall = ScreenManager::North_;
}

///-------------------------------------
/// Brief: door copy constructor
Door::Door(Door& ogDoor, std::string area)
{
  Name(ogDoor.Name());
  startState_ = ogDoor.startState_;
  Start();
  area = area;
  ID = "TEST";
  wall = ScreenManager::North_;
}

///-------------------------------------
/// Brief: returns starting state
dState Door::StartState()
{
  return startState_;
}

///-------------------------------------
/// Brief: sets starting state
///        also updates current state
void Door::StartState(dState state)
{
  StateNext(state);
  startState_ = state;
}

dState GetDoorStateFromString(std::string state)
{
  dState d = dInvalid;

  if (state.compare("open") == 0)
    d = dOpen;
  else if (state.compare("locked") == 0)
    d = dLocked;
  else if (state.compare("unlocked") == 0)
    d = dUnlocked;
  else if (state.compare("itsatrap") == 0)
    d = dItsATrap;

  return d;
}
