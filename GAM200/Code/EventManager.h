///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Event.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "Events.h"
#include "Spawner.h"
#include "Spaces.h"
#include "System.h"
//#include "Door.h"
#include "Menu.h"

///----------------------------------GLOBALS------------------------------------

///----------------------------------CLASSES------------------------------------
class EventManager : public System
{
public:
  EventManager();
  ~EventManager();
  virtual void Initialize();
  virtual void Update(float dt);

  static EventManager * GetInstance();

  void SendPlayerCollisionEvent(std::string objName); //this calls the object's OnCollisionWithPlayer
  void SendCollisionEvent(std::string objName, Collider& other); 
  void SendDeactivateEvent(std::string objName); //this calls the object's OnDeactivate
  void SendSetVisibleEvent(std::string objName, bool vis);
  void SendSetActiveEvent(std::string objName, bool active);
  //void SendCompCollisionEvent(int ID);

  void IncreaseEventComps();
  int NumEventComps();

  EventZone::Event<bool> * GetIsAggroedEvent(std::string enemyName);
  EventZone::Event<bool> * GetSetVisibleEvent(std::string objName);
  EventZone::Event<bool> * GetSetActiveEvent(std::string objName);
  EventZone::Event<bool> * GetSetInvincibleEvent(std::string objName);

  EventZone::Event<SpawnData> spawn; //spawns an obj at a location
  EventZone::Event<DoorData> setDoor; //sets the state of a door
  EventZone::Event<PopupParam> popup; //create a popup window
  EventZone::Event<SoundData> setSound; //changes the background music
  EventZone::Event<void> toggleDark; 
  EventZone::Event<colorFlag> catPlus; //increases the color of catalyst by 1
  EventZone::Event<colorFlag> catMin; //decreases the color of catalyst by 1
  EventZone::Event<CatData> cats; //adjusts the color of catalyst by amount
  EventZone::Event<int> playerHP; //adjusts player health by int amount

  EventZone::Event<bool> setTickTocks;
  //EventZone::Event<bool> bossEvent;

private:
  std::map<std::string, EventZone::Event<void>> playerCollisionEvents_;
  std::map<std::string, EventZone::Event<Collider>> collisionEvents_;
  std::map<std::string, EventZone::Event<void>> deactivateEvents_;

  std::map<std::string, EventZone::Event<bool>> setVisibleEvents_;
  std::map<std::string, EventZone::Event<bool>> isAggroedEvents_;
  std::map<std::string, EventZone::Event<bool>> setActiveEvents_;
  std::map<std::string, EventZone::Event<bool>> setInvincibleEvents_;

  //std::multimap< std::string, EventZone::Event<Collider> > compCollisionEvents_;

  int numEventComps_;
};

///---------------------------------FUNCTIONS-----------------------------------
void AddTickTockToTickTocks(std::string name);
void SetTickTockVisible(bool& vis);
void BossRoomEvent(bool start);
//void NecromancerCallFunction(bool& set);

//template<typename T>
//EventZone::Event<T> * GetEventByNumber(eventNum num)
//{
//  if (num == SetTickTocks_)
//    return &(EventManager::GetInstance()->setTickTocks);
//}
