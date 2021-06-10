///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Spawn.h
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

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class Spawn : public EventComp<SpawnData>
{
public:
  Spawn(trigType type, SpawnData& data);
  Spawn(SpawnData& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  Spawn(SpawnData& data, resType collisionT = Player_, trigType type = OnCollision_);
  Spawn(Spawn& ogSpawn);

  void SetData(SpawnData& data);
  float X();
  float Y();
  std::string ObjType();
  bool ByName();

private:


};

///---------------------------------FUNCTIONS-----------------------------------


