///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: GameGs.cpp
//
//  Author: Megan Works
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
#include "Spaces.h"
#include "Engine.h"
#include "ScreenManager.h"
#include "GameGs.h"
#include "Spawner.h"
#include "EventManager.h"
#include "Trigger.h"
#include "Spawn.h"
#include "ObjManager.h"

///----------------------------------GLOBALS-----------------------------------
Trigger *trig;
Spawn *spawn;
SpawnData data;
static ScreenManager *screenman;

///---------------------------------FUNCTIONS-----------------------------------

void GameGsLoad()
{
  // temp while no gamestates
  screenman = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
  // screenman->ChangeCurrentScreen(screenman->StartScreen());
}

void GameGsInit()
{
  
}

void GameGsUpdate(float dt)
{

}

void GameGsShutdown()
{
}

void GameGsUnload()
{
}
