///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Main.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: It's main. Look there it is. 
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------

#include "stdafx.h"
#include "Engine.h"

///----------------------------------GLOBALS-----------------------------------

int main(void)
{
#ifndef _DEBUG
  FreeConsole();
#endif // !_DEBUG


  // Change in time (in seconds) since the last game loop.
  float dt = 0;

  // Initialize the game engine.
  EngineInit();

  // MAIN LOOP
  EngineUpdate(dt);

  // Shutdown the game engine.
  EngineShutdown();

  return 0;
}