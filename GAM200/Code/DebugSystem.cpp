///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: DebugSystem.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This class handles the push blocks used for puzzles
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "DebugSystem.h"


///----------------------------------GLOBALS------------------------------------
static ObjectList objs;
static ScreenManager* sm;
///----------------------------------CLASSES------------------------------------

void DebugSystem::Initialize()
{
  sm = reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
}

void DebugSystem::Update(float dt)
{
  if (isDirty_)
  {
    objs = sm->CurrentObjList();

    for (auto elem : objs)
    {
      Debug* debug = elem->Has(Debug);
      if (debug)
        debug->updateMode();
    }

    Debug* playerDebug = Space::GetPlayer().Has(Debug);
    if (playerDebug)
      playerDebug->updateMode();

    isDirty_ = false;
  }
}
