///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ChangeMusic.cpp
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
#include "ChangeMusic.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
ChangeMusic::ChangeMusic(trigType type, SoundData & data) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->setSound));

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  Type(ChangeMusic_);
}

ChangeMusic::ChangeMusic(SoundData & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->setSound));
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  for (std::string s : rooms)
    AddRoomNeeded(s);

  Type(ChangeMusic_);
}

ChangeMusic::ChangeMusic(SoundData & data, resType collisionT, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  Event(&(man->setSound));
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  CollisionType(collisionT);

  Type(ChangeMusic_);
}

ChangeMusic::ChangeMusic(ChangeMusic & ogChangeMusic) : EventComp(ogChangeMusic.TriggerType(), ogChangeMusic.Data())
{
  *this = ogChangeMusic;

  //EventManager *man = EventManager::GetInstance();
  //ID(man->NumEventComps());
  //man->IncreaseEventComps();
}

std::string ChangeMusic::Filename()
{
  return Data().filename;
}

bool ChangeMusic::Play()
{
  return Data().play;
}
