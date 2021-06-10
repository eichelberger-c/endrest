///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Aggro.cpp
//
//  Author: Sonya Toft
//
//  Copyright � 2018 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Aggro.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
Aggro::Aggro(trigType type, AggroData & data) : EventComp(type, data.toggle)
{
  EventManager *man = EventManager::GetInstance();
  Event((man->GetIsAggroedEvent(data.enemyName)));

  ID(man->NumEventComps());
  man->IncreaseEventComps();

  Type(Aggro_);
}

Aggro::Aggro(AggroData & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data.toggle)
{
  EventManager *man = EventManager::GetInstance();
  Event((man->GetIsAggroedEvent(data.enemyName)));

  ID(man->NumEventComps());
  man->IncreaseEventComps();

  for (std::string s : rooms)
    AddRoomNeeded(s);

  Type(Aggro_);
}

Aggro::Aggro(AggroData & data, resType collisionT, trigType type) : EventComp(type, data.toggle)
{
  EventManager *man = EventManager::GetInstance();
  Event((man->GetIsAggroedEvent(data.enemyName)));

  ID(man->NumEventComps());
  man->IncreaseEventComps();

  CollisionType(collisionT);

  Type(Aggro_);
}

Aggro::Aggro(Aggro & ogAggro) : EventComp(ogAggro.TriggerType(), ogAggro.Data())
{
  *this = ogAggro;

  EventManager *man = EventManager::GetInstance();
  ID(man->NumEventComps());
  man->IncreaseEventComps();

  //Event(ogAggro.Event());
  //Type(ogAggro.Type());
  //CollisionType(ogAggro.CollisionType());
}
