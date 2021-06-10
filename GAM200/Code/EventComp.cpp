///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: EventComp.cpp
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
#include "EventComp.h"
#include "ScreenManager.h"
#include "Engine.h"
//#include "Trigger.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------


template<typename T>
EventComp<T>::EventComp(trigType type, T & data) : Component(IsEvent_)
{
  trigType_ = type;
  if (type == OnCollision_)
  {
    collisionType_ = Player_;
  }

  data_ = data;
}

template<typename T>
EventComp<T>::EventComp(trigType type, EventZone::Event<T> *eventP, T& data)
{
  trigType_ = type;
  data_ = data;
  if (type == OnCollision_)
  {
    collisionType_ = Player_;
  }

  event_ = eventP;
}

template<typename T>
EventComp<T>::EventComp(trigType type) : Component(IsEvent_)
{
  trigType_ = type;
  data_ = 0; //void   (NULL? nullptr? ???)
  if (type == OnCollision_)
  {
    collisionType_ = Player_;
  }
}

template<typename T>
EventComp<T>::EventComp(const EventComp<T>& ogEventComp) : Component(ogEventComp.type_)
{
  isTriggered_ = ogEventComp.isTriggered_;
  data_ = ogEventComp.data_;
  canReset_ = ogEventComp.canReset_;
  event_ = ogEventComp.event_;
  trigType_ = ogEventComp.trigType_;
  collisionType_ = ogEventComp.collisionType_;
  roomsNeeded_ = ogEventComp.roomsNeeded_;
  allRoomsComplete_ = ogEventComp.allRoomsComplete_;

  if (trigType_ == OnCollision_)
    collisionType_ = Player_;
}

template<typename T>
Component * EventComp<T>::Clone() const
{
  T data = data_;
  EventComp<T> *clone = new EventComp<T>(trigType_, data);
  if (clone == nullptr)
    return nullptr;

  *clone = *this;

  return clone;
}

template<typename T>
void EventComp<T>::Update(float dt)
{
  if ((isTriggered_ && Type() != FlashSprite_) || !IsActive())
    return;

  switch (trigType_)
  {
  case OnCollision_:
    break;
  case OnRoomsComplete_:
  {
    UpdateRooms();
    CheckRoomsComplete();

    if (allRoomsComplete_)
    {
      EventZone::Event<T>& ev = *event_;
      ev(data_);
      isTriggered_ = true;
    }
    break;
  }
  case OnEnemiesDead_:
  {
    ScreenManager *man = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
    Screen *screen = man->CurrentScreen();
    if (screen->AllEnemiesDead())
    {
      EventZone::Event<T>& ev = *event_;
      ev(data_);
      isTriggered_ = true;
    }
    break;
  }
  case OnTimer_:
  {
    Timer *timothy = Parent()->Has(Timer);
    if (timothy->Beep())
    {
      EventZone::Event<T>& ev = *event_;
      ev(data_);
      isTriggered_ = true;
    }
    break;
  }

  }
}

template<typename T>
void EventComp<T>::Reset()
{
  isTriggered_ = false;
}

template<typename T>
void EventComp<T>::UpdateRooms()
{
  ScreenManager *man = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));

  for (auto room : roomsNeeded_)
    roomsNeeded_[room.first] = man->ScreenIsComplete(room.first);
}

template<typename T>
bool EventComp<T>::CheckRoomsComplete()
{
  for (auto room : roomsNeeded_)
    if (!room.second)
      return false;

  allRoomsComplete_ = true;

  return true;
}

template<typename T>
void EventComp<T>::OnCollision(Collider & other)
{
  if (isTriggered_ || trigType_ != OnCollision_ || collisionType_ == No_)
    return;

  if (other.Parent()->ResolutionType() == collisionType_)
  {
    EventZone::Event<T>& ev = *event_;
    ev(data_);
    isTriggered_ = true;
  }
}

template<typename T>
void EventComp<T>::OnCollision(int type)
{
  if (isTriggered_ || trigType_ != OnCollision_)
    return;

  if (type == collisionType_)
  {
    EventZone::Event<T>& ev = *event_;
    ev(data_);
    isTriggered_ = true;
  }
}

template<typename T>
bool EventComp<T>::IsTriggered() const
{
  return isTriggered_;
}

template<typename T>
void EventComp<T>::IsTriggered(bool toggle)
{
  isTriggered_ = toggle;
}

template<typename T>
T & EventComp<T>::Data()
{
  return data_;
}

template<typename T>
T EventComp<T>::Data() const
{
  return data_;
}

template<typename T>
bool EventComp<T>::CanReset() const
{
  return canReset_;
}

template<typename T>
void EventComp<T>::CanReset(bool toggle)
{
  canReset_ = toggle;
}

template<typename T>
EventZone::Event<T>* EventComp<T>::Event()
{
  return event_;
}

template<typename T>
void EventComp<T>::Event(EventZone::Event<T>* newEvent)
{
  event_ = newEvent;
}

template<typename T>
trigType EventComp<T>::TriggerType() const
{
  return trigType_;
}

template<typename T>
resType EventComp<T>::CollisionType() const
{
  return collisionType_;
}

template<typename T>
void EventComp<T>::CollisionType(resType type)
{
  collisionType_ = type;
}

template<typename T>
bool EventComp<T>::AllRoomsComplete() const
{
  return allRoomsComplete_;
}

template<typename T>
void EventComp<T>::AllRoomsComplete(bool toggle)
{
  allRoomsComplete_ = toggle;
}

template<typename T>
RoomsNeeded * EventComp<T>::RoomsNeeded()
{
  return &roomsNeeded_;
}

template<typename T>
void EventComp<T>::AddRoomNeeded(std::string screenName)
{
  roomsNeeded_[screenName] = false;
  //CONSIDER: reset isTriggered_ so that it'll check for the new room? 
}

template<typename T>
void EventComp<T>::RemoveRoomNeeded(std::string screenName)
{
  roomsNeeded_.erase(screenName);
}

template<typename T>
void EventComp<T>::RoomComplete(std::string name)
{
  roomsNeeded_[name] = true;
}

