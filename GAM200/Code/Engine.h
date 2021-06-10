///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Engine.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include "System.h"
#include "Utils.h"
#include "AudioEngine.h"

///----------------------------------GLOBALS-----------------------------------

///----------------------------------CLASSES------------------------------------
class Engine
{
public:
  void AddSystem(System *system);
  void DestroySystems();
  void SystemsInit();
  void GameLoop(float dt);

  void CheckMinimized();

  friend System* EngineGetSystem(std::string name);
  friend void PlayerInputIsActive(bool toggle);
  friend void Pause();
  friend void UnPause();

private:
  void UpdateSystems(float dt);
  //BroadcastMessage funtion
  float time_ = 0;
  std::map<std::string, System*>systems_;  

  bool paused_ = false;
  float delay_ = 0.f;
};


///---------------------------------FUNCTIONS-----------------------------------

// Initialize the game engine.
void EngineInit();

// Update the game engine
void EngineUpdate(float dt);

// Shutdown the game engine.
void EngineShutdown();

// pause the game
void Pause();

// unpause the game
void UnPause();

System* EngineGetSystem(std::string name);

void PlayerInputIsActive(bool toggle);

std::string EngineGetCurrentScreen();


void SoundEvent(SoundData& data);
/*----------------------------------------------------------------------------*/

