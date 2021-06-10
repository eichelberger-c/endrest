///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ChangeMusic.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "EventComp.h"
#include "Spawner.h"
#include "Engine.h"
#include "Utils.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class ChangeMusic : public EventComp<SoundData>
{
public:
  ChangeMusic(trigType type, SoundData& data);
  ChangeMusic(SoundData& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  ChangeMusic(SoundData& data, resType collisionT = Player_, trigType type = OnCollision_);
  ChangeMusic(ChangeMusic& ogChangeMusic);

  std::string Filename();
  bool Play();

private:

};

///---------------------------------FUNCTIONS-----------------------------------


