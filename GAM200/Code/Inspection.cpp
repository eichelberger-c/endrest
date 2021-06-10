///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Inspection.cpp
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
#include "Inspection.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
Inspection::Inspection(trigType type, PopupParam & data) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->popup));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  Type(Inspection_);
}

Inspection::Inspection(PopupParam & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->popup));
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  for (std::string s : rooms)
    AddRoomNeeded(s);

  Type(Inspection_);
}

Inspection::Inspection(PopupParam & data, resType collisionT, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->popup));
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  CollisionType(collisionT);

  Type(Inspection_);
}

Inspection::Inspection(Inspection & ogInspection) : EventComp(ogInspection.TriggerType(), ogInspection.Data())
{
  *this = ogInspection;

  EventManager *man = EventManager::GetInstance();
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  //Event(ogInspection.Event());
  //Type(ogInspection.Type());
}

void Inspection::SetData(PopupParam & data)
{
  Data(data);
}

std::string Inspection::Text()
{
  return Data().text;
}

std::string Inspection::ObjName()
{
  return Data().obj->Name();
}

int Inspection::Length()
{
  return Data().length;
}

int Inspection::Height()
{
  return Data().height;
}

float Inspection::Time()
{
  return Data().time;
}

