///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: FlashSprite.cpp
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
#include "FlashSprite.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------


FlashSprite::FlashSprite(trigType type, FlashData & data) : EventComp(type)
{
  Type(FlashSprite_);
  fd_ = data;
  tom_.MaxTime(fd_.duration);
  tom_.Time(fd_.duration);

  ev_ += EventZone::EventHandler::Bind(&FlashSprite::Flash, this);
  Event(&ev_);
}

FlashSprite::FlashSprite(FlashData & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type)
{
  Type(FlashSprite_);
  fd_ = data;
  tom_.MaxTime(fd_.duration);
  tom_.Time(fd_.duration);

  ev_ += EventZone::EventHandler::Bind(&FlashSprite::Flash, this);
  Event(&ev_);

  for (std::string s : rooms)
    AddRoomNeeded(s);
}

FlashSprite::FlashSprite(FlashData & data, resType collisionT, trigType type) : EventComp(type)
{
  Type(FlashSprite_);
  fd_ = data;
  tom_.MaxTime(fd_.duration);
  tom_.Time(fd_.duration);

  ev_ += EventZone::EventHandler::Bind(&FlashSprite::Flash, this);
  Event(&ev_);
}

FlashSprite::FlashSprite(FlashSprite & ogFlashSprite) : EventComp(ogFlashSprite.TriggerType())
{
  Type(FlashSprite_);
  fd_ = ogFlashSprite.fd_;
  tom_.MaxTime(fd_.duration);
  tom_.Time(fd_.duration);

  ev_ += EventZone::EventHandler::Bind(&FlashSprite::Flash, this);
  Event(&ev_);
}

FlashSprite::~FlashSprite()
{
  ev_ -= EventZone::EventHandler::Bind(&FlashSprite::Flash, this);
}

void FlashSprite::Update(float dt)
{
  if (!flash_ || !IsActive())
    return;

  if (IsTriggered())
    IsTriggered(false);

  EventComp::Update(dt);

  if (flash_ == true)
  {
    tom_.Update(dt);

    if (tom_.Beep() == true)
    {
      EventManager *man = EventManager::GetInstance();
      man->SendSetVisibleEvent(fd_.objName, false);
      flash_ = false;
      IsTriggered(true);
    }
  }
}

void FlashSprite::Flash(bool& decoy)
{
    //stupid >:[
  decoy = true; 
  flash_ = decoy;

  EventManager *man = EventManager::GetInstance();
  man->SendSetVisibleEvent(fd_.objName, true);
}

void FlashSprite::SetData(FlashData& data)
{
  fd_ = data;
  tom_.MaxTime(fd_.duration);
  tom_.Time(fd_.duration);
}

std::string FlashSprite::ObjName()
{
  return fd_.objName;
}

secs FlashSprite::Duration()
{
  return fd_.duration;
}
