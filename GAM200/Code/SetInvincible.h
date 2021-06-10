///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SetInvincible.h
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
#include "EventComp.h"
#include "Spawner.h"
#include "Engine.h"
#include "Utils.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class SetInvincible : public EventComp<bool>
{
public:
  SetInvincible(trigType type, BoolEventData& data);
  SetInvincible(BoolEventData& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  SetInvincible(BoolEventData& data, resType collisionT = Player_, trigType type = OnCollision_);
  SetInvincible(SetInvincible& ogSetInvincible);

  void SetData(BoolEventData& data);
  void ObjName(std::string name);
  std::string ObjName();
  bool Bool();
  virtual void ReGetEvent() override;

private:
  std::string objName_;
};

///---------------------------------FUNCTIONS-----------------------------------


