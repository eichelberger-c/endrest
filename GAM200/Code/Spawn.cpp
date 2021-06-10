///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Spawn.cpp
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
#include "Spawn.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

Spawn::Spawn(trigType type, SpawnData & data) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->spawn));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  Type(Spawn_);
}

Spawn::Spawn(SpawnData & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->spawn));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  for (std::string s : rooms)
    AddRoomNeeded(s);

  Type(Spawn_);
}

Spawn::Spawn(SpawnData & data, resType collisionT, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->spawn));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  CollisionType(collisionT);

  Type(Spawn_);
}

Spawn::Spawn(Spawn & ogSpawn) : EventComp(ogSpawn.TriggerType(), ogSpawn.Data())
{
  *this = ogSpawn;

  EventManager *man = EventManager::GetInstance();
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  //Event(ogSpawn.Event());
  //Type(ogSpawn.Type());
}

void Spawn::SetData(SpawnData & data)
{
  Data(data);
}

float Spawn::X()
{
  return Data().position.X();
}

float Spawn::Y()
{
  return Data().position.Y();
}

std::string Spawn::ObjType()
{
  return Data().type;
}

bool Spawn::ByName()
{
  return Data().byName;
}
