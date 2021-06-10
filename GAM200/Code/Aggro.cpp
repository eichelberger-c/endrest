///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Aggro.cpp
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
#include "Aggro.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
Aggro::Aggro(trigType type, BoolEventData & data) : EventComp(type, data.state)
{
  //EventManager *man = EventManager::GetInstance();
  //Event(man->GetIsAggroedEvent(data.objName));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  enemyName_ = data.objName;
  Type(Aggro_);
}

Aggro::Aggro(BoolEventData & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data.state)
{
  EventManager *man = EventManager::GetInstance();
  Event((man->GetIsAggroedEvent(data.objName)));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  for (std::string s : rooms)
    AddRoomNeeded(s);

  enemyName_ = data.objName;
  Type(Aggro_);
}

Aggro::Aggro(BoolEventData & data, resType collisionT, trigType type) : EventComp(type, data.state)
{
  EventManager *man = EventManager::GetInstance();
  Event((man->GetIsAggroedEvent(data.objName)));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  CollisionType(collisionT);

  enemyName_ = data.objName;
  Type(Aggro_);
}

Aggro::Aggro(Aggro & ogAggro) : EventComp(ogAggro.TriggerType(), ogAggro.Data())
{
  *this = ogAggro;

  ////ID(man->NumEventComps());
  ////man->IncreaseEventComps();

  //Event(ogAggro.Event());
  //Type(ogAggro.Type());
  //CollisionType(ogAggro.CollisionType());
}

void Aggro::SetData(BoolEventData data)
{
  Data(data.state);
  enemyName_ = data.objName;
}

void Aggro::EnemyName(std::string name)
{
  enemyName_ = name;
}

std::string Aggro::EnemyName()
{
  return enemyName_;
}

bool Aggro::Bool()
{
  return Data();
}

void Aggro::ReGetEvent()
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetIsAggroedEvent(enemyName_));
}
