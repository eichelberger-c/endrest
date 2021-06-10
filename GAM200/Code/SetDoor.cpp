///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SetDoor.cpp
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
#include "SetDoor.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
SetDoor::SetDoor(trigType type, DoorData & data) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->setDoor));

  ////ID(man->NumEventComps());
  ////man->IncreaseEventComps();

  Type(SetDoor_);
}

SetDoor::SetDoor(DoorData & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->setDoor));
  ////ID(man->NumEventComps());
  ////man->IncreaseEventComps();

  for (std::string s : rooms)
    AddRoomNeeded(s);

  Type(SetDoor_);
}

SetDoor::SetDoor(DoorData & data, resType collisionT, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->setDoor));
  ////ID(man->NumEventComps());
  ////man->IncreaseEventComps();

  CollisionType(collisionT);

  Type(SetDoor_);
}

SetDoor::SetDoor(SetDoor & ogSetDoor) : EventComp(ogSetDoor.TriggerType(), ogSetDoor.Data())
{
  *this = ogSetDoor;

  //EventManager *man = EventManager::GetInstance();
  ////ID(man->NumEventComps());
  ////man->IncreaseEventComps();

  //Event(ogSetDoor.Event());
  //Type(ogSetDoor.Type());
}

void SetDoor::SetData(DoorData & data)
{
  Data(data);
}

int SetDoor::NewState()
{
  return Data().newState;
}

std::string SetDoor::DoorID()
{
  return Data().doorID;
}

std::string SetDoor::ScreenName()
{
  return Data().screen;
}

std::string GetStringFromDoorState(int state)
{
  switch (state)
  {
  case 1:
    return "open";
  case 2:
    return "locked";
  case 3:
    return "unlocked";
  case 4:
    return "itsatrap";
  }

  return "open";
}
