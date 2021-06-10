///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SetInvincible.cpp
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
#include "SetInvincible.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
SetInvincible::SetInvincible(trigType type, BoolEventData & data) : EventComp(type, data.state)
{
  //EventManager *man = EventManager::GetInstance();
  //Event(man->GetSetInvincibleEvent(data.objName));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  objName_ = data.objName;
  Type(SetInvincible_);
}

SetInvincible::SetInvincible(BoolEventData & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data.state)
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetSetInvincibleEvent(data.objName));
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  for (std::string s : rooms)
    AddRoomNeeded(s);

  objName_ = data.objName;
  Type(SetInvincible_);
}

SetInvincible::SetInvincible(BoolEventData & data, resType collisionT, trigType type) : EventComp(type, data.state)
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetSetInvincibleEvent(data.objName));
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  CollisionType(collisionT);

  objName_ = data.objName;
  Type(SetInvincible_);
}

SetInvincible::SetInvincible(SetInvincible & ogSetInvincible) : EventComp(ogSetInvincible.TriggerType(), ogSetInvincible.Data())
{
  *this = ogSetInvincible;

  //EventManager *man = EventManager::GetInstance();
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  //Event(ogSetInvincible.Event());
  //Type(ogSetInvincible.Type());
}

void SetInvincible::SetData(BoolEventData & data)
{
  Data(data.state);
  objName_ = data.objName;
}

void SetInvincible::ObjName(std::string name)
{
  objName_ = name;
}

std::string SetInvincible::ObjName()
{
  return objName_;
}

bool SetInvincible::Bool()
{
  return Data();
}

void SetInvincible::ReGetEvent()
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetSetInvincibleEvent(objName_));
}
