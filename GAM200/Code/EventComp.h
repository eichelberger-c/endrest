///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: EventComp.h
//
//  Author: Sonya Toft
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - EventComp
//      - EventComp (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"
#include "Area.h"
#include "Obj.h"
#include "Collider.h"

#include <initializer_list>

///----------------------------------GLOBALS------------------------------------
//using RoomsNeeded = std::map<std::string, bool>;
typedef std::map<std::string, bool> RoomsNeeded;

typedef enum EventTriggerTypes
{
  OnCollision_,
  OnRoomsComplete_, //1
  OnEnemiesDead_,
  OnTimer_,

} trigType;

typedef enum EventNumbers
{
  SetTickTocks_,

} eventNum;

typedef bool(*TriggerFunc)(); //may potentially use later

///----------------------------------CLASSES------------------------------------

template<typename T>
class EventComp : public Component
{
public:
  EventComp(trigType type, T& data);
  EventComp(trigType type, EventZone::Event<T> *eventP, T& data);

  EventComp(trigType type); //only for EventComps where the data is void

  EventComp(const EventComp& ogEventComp);
  virtual Component * Clone() const;
  virtual void Update(float dt) override;
  virtual void Reset() override;

  void UpdateRooms();
  bool CheckRoomsComplete();

  void Customize(EventZone::Event<T> *newEvent)
  {
    isCustomized_ = true;
    event_ = newEvent;
  }

  virtual void OnCollision(Collider& other) override;
  virtual void OnCollision(int type) override;

  bool IsTriggered() const;
  void IsTriggered(bool toggle);
  T& Data();
  T Data() const;
  void Data(T& data)
  {
    data_ = data;
  }
  bool CanReset() const;
  void CanReset(bool toggle);
  EventZone::Event<T> * Event();
  void Event(EventZone::Event<T> *newEvent); //I dunno if there's any point to this
  trigType TriggerType() const;
  void TriggerType(trigType type)
  {
    trigType_ = type;
  }
  resType CollisionType() const;
  void CollisionType(resType type);
  bool AllRoomsComplete() const;
  void AllRoomsComplete(bool toggle);

  bool IsCustomized()
  {
    return isCustomized_;
  }

  void IsCustomized(bool set)
  {
    isCustomized_ = set;
  }
  //int ID();
  //void ID(int ID);

  RoomsNeeded * RoomsNeeded();
  void RoomsNeeded(std::vector<std::string> rooms)
  {
    for (std::string s : rooms)
      roomsNeeded_[s] = false;
  }
  void AddRoomNeeded(std::string screenName);
  void RemoveRoomNeeded(std::string screenName);

  virtual void DoTheEvent()
  {
    EventZone::Event<T>& ev = *event_;
    ev(data_);
    isTriggered_ = true;
  }

  virtual void ReGetEvent() {}

  void GetInfoFromParent()
  {
    Trigger *parent = static_cast<Trigger*>(Parent());

    trigType_ = parent->GetTriggerType();
    if (trigType_ == OnCollision_)
      collisionType_ = parent->GetCollisionType();
    else if (trigType_ == OnRoomsComplete_)
      RoomsNeeded(parent->GetRoomsNeeded());
  }

  friend class Trigger;

private:
  bool isTriggered_ = false; //if it's been triggered or not
  T data_; //data to pass to the event
  bool canReset_ = false;
  EventZone::Event<T> *event_; //pointer to the event to be called
  trigType trigType_; //what type of thing triggers this event

  resType collisionType_ /*= Player_*/; //if OnCollision, collision with what

  std::map<std::string, bool> roomsNeeded_; //if OnRooms, which rooms
  bool allRoomsComplete_ = false;

  bool isCustomized_;

  //int ID_;

  void RoomComplete(std::string name);
};

///---------------------------------FUNCTIONS-----------------------------------

