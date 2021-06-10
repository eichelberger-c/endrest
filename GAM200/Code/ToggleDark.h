///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ToggleDark.h
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
class ToggleDark : public EventComp<bool>
{
public:
  ToggleDark(trigType type, bool& data);
  ToggleDark(bool& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  ToggleDark(bool& data, resType collisionT = Player_, trigType type = OnCollision_);
  ToggleDark(ToggleDark& ogToggleDark);

  virtual void Update(float dt) override;
  virtual void OnCollision(Collider& other) override;

private:
  EventZone::Event<void> *ev_;
};

///---------------------------------FUNCTIONS-----------------------------------


