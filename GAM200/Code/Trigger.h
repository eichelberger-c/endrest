///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Trigger.h
//
//  Author: Sonya Toft
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Brief:
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "Obj.h"
#include "Component.h"
#include "EventManager.h"
#include "Vec2D.h"
#include "Collider.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class Trigger : public Obj
{
public:
  Trigger();
  //Trigger(Vec2D position = { 0.0f, 0.0f }, float colliderRadius = 36.0f);
  Trigger(const Obj& papaObj, std::string name);
  virtual void Update(float dt);

  virtual void Reset() override;
  void SetTriggerType(trigType t);
  void SetCollisionType(resType t);
  void SetRoomsNeeded(std::vector<std::string> rooms);
  
  void Collide(Collider& other);
  void Collide(resType type);

  trigType GetTriggerType();
  resType GetCollisionType();
  std::vector<std::string> GetRoomsNeeded();
  void TriggerType(trigType type);
  void CollisionType(resType type);
  void Rooms(std::vector<std::string> rooms);

  void DoTheEvents();

private:
  bool isTriggered_;
  trigType trig_;
  resType res_;

  std::vector<std::string>rooms_;
};

///---------------------------------FUNCTIONS-----------------------------------


