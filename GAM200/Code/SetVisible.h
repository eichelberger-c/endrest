///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SetVisible.h
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
class SetVisible : public EventComp<bool>
{
public:
  SetVisible(trigType type, BoolEventData& data);
  SetVisible(BoolEventData& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  SetVisible(BoolEventData& data, resType collisionT = Player_, trigType type = OnCollision_);
  SetVisible(SetVisible& ogSetVisible);

  void SetData(BoolEventData& data);
  void ObjName(std::string name);
  std::string ObjName();
  bool Bool();

  virtual void ReGetEvent();

private:
  std::string objName_;
};

///---------------------------------FUNCTIONS-----------------------------------


