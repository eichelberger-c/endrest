///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ToggleDark.cpp
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
#include "ToggleDark.h"
#include "EventManager.h"
#include "EventComp.cpp"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
ToggleDark::ToggleDark(trigType type, bool & data) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  ev_ = &(man->toggleDark);

  //ID(man->NumEventComps());
  //man->IncreaseEventComps();

  Type(ToggleDark_);
}

ToggleDark::ToggleDark(bool & data, std::initializer_list<std::string> rooms, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  ev_ = &(man->toggleDark);

  for (std::string s : rooms)
    AddRoomNeeded(s);

  Type(ToggleDark_);
}

ToggleDark::ToggleDark(bool & data, resType collisionT, trigType type) : EventComp(type, data)
{
  EventManager *man = EventManager::GetInstance();
  ev_ = &(man->toggleDark);

  CollisionType(collisionT);

  Type(ToggleDark_);
}

ToggleDark::ToggleDark(ToggleDark & ogToggleDark) : EventComp(ogToggleDark.TriggerType(), ogToggleDark.Data())
{
  ev_ = ogToggleDark.ev_;

  EventManager *man = EventManager::GetInstance();
  ev_ = &(man->toggleDark);
  Type(ToggleDark_);
}

void ToggleDark::Update(float dt)
{
  if ((IsTriggered() && Type() != FlashSprite_) || !IsActive())
    return;

  trigType type = TriggerType();
  switch (type)
  {
  case OnCollision_:
    break;
  case OnRoomsComplete_:
  {
    UpdateRooms();
    CheckRoomsComplete();

    if (AllRoomsComplete())
    {
      EventZone::Event<void>& ev = *ev_;
      ev();
      IsTriggered(true);
    }
    break;
  }
  case OnEnemiesDead_:
  {
    ScreenManager *man = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
    Screen *screen = man->CurrentScreen();
    if (screen->AllEnemiesDead())
    {
      EventZone::Event<void>& ev = *ev_;
      ev();
      IsTriggered(true);
    }
    break;
  }
  case OnTimer_:
  {
    Timer *timothy = Parent()->Has(Timer);
    if (timothy->Beep())
    {
      EventZone::Event<void>& ev = *ev_;
      ev();
      IsTriggered(true);
    }
    break;
  }
  }
}

void ToggleDark::OnCollision(Collider & other)
{
  if (IsTriggered() || TriggerType() != OnCollision_ || CollisionType() == No_)
    return;

  if (other.Parent()->ResolutionType() == CollisionType())
  {
    EventZone::Event<void>& ev = *ev_;
    ev();
    IsTriggered(true);
  }
}
