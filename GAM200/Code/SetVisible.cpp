///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SetVisible.cpp
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
#include "SetVisible.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
SetVisible::SetVisible(trigType type, BoolEventData & data) : EventComp(type, data.state)
{
  //EventManager *man = EventManager::GetInstance();
  //Event(man->GetSetVisibleEvent(data.objName));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  objName_ = data.objName;
  Type(SetVisible_);
}

SetVisible::SetVisible(BoolEventData & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data.state)
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetSetVisibleEvent(data.objName));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  for (std::string s : rooms)
    AddRoomNeeded(s);

  objName_ = data.objName;
  Type(SetVisible_);
}

SetVisible::SetVisible(BoolEventData & data, resType collisionT, trigType type) : EventComp(type, data.state)
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetSetVisibleEvent(data.objName));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  CollisionType(collisionT);

  objName_ = data.objName;
  Type(SetVisible_);
}

SetVisible::SetVisible(SetVisible & ogSetVisible) : EventComp(ogSetVisible.TriggerType(), ogSetVisible.Data())
{
  *this = ogSetVisible;

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  //Event(ogSetVisible.Event());
  //Type(ogSetVisible.Type());
}

void SetVisible::SetData(BoolEventData & data)
{
  Data(data.state);
  objName_ = data.objName;
}

void SetVisible::ObjName(std::string name)
{
  objName_ = name;
}

std::string SetVisible::ObjName()
{
  return objName_;
}

bool SetVisible::Bool()
{
  return Data();
}

void SetVisible::ReGetEvent()
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetSetVisibleEvent(objName_));
}
