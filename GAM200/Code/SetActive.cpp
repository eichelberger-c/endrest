///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SetActive.cpp
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
#include "SetActive.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
SetActive::SetActive(trigType type, BoolEventData & data) : EventComp(type, data.state)
{
  //EventManager *man = EventManager::GetInstance();
  //Event(man->GetSetActiveEvent(data.objName));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  objName_ = data.objName;
  Type(SetActive_);
}

SetActive::SetActive(BoolEventData & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data.state)
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetSetActiveEvent(data.objName));
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  for (std::string s : rooms)
    AddRoomNeeded(s);

  objName_ = data.objName;
  Type(SetActive_);
}

SetActive::SetActive(BoolEventData & data, resType collisionT, trigType type) : EventComp(type, data.state)
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetSetActiveEvent(data.objName));
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  CollisionType(collisionT);

  objName_ = data.objName;
  Type(SetActive_);
}

SetActive::SetActive(SetActive & ogSetActive) : EventComp(ogSetActive.TriggerType(), ogSetActive.Data())
{
  *this = ogSetActive;

  //EventManager *man = EventManager::GetInstance();
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  //Event(ogSetActive.Event());
  //Type(ogSetActive.Type());
}

void SetActive::SetData(std::string name, bool b)
{
  Data(b);
  objName_ = name;
}

void SetActive::SetData(BoolEventData data)
{
  Data(data.state);
  objName_ = data.objName;
}

//void SetActive::SetDataActive(BoolEventData & data)
//{
//  Data(data.state);
//  objName_ = data.objName;
//}

void SetActive::ObjName(std::string name)
{
  objName_ = name;
}

std::string SetActive::ObjName()
{
  return objName_;
}

bool SetActive::Bool()
{
  return Data();
}

void SetActive::ReGetEvent()
{
  EventManager *man = EventManager::GetInstance();
  Event(man->GetSetActiveEvent(objName_));
}
