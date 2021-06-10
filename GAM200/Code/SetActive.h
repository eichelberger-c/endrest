///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SetActive.h
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
class SetActive : public EventComp<bool>
{
public:
  SetActive(trigType type, BoolEventData& data);
  SetActive(BoolEventData& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  SetActive(BoolEventData& data, resType collisionT = Player_, trigType type = OnCollision_);
  SetActive(SetActive& ogSetActive);

  void SetData(std::string name, bool b);
  void SetData(BoolEventData data);
  //void SetDataActive(BoolEventData& data);
  void ObjName(std::string name);
  std::string ObjName();
  bool Bool();
  virtual void ReGetEvent();

private:
  std::string objName_;
};

///---------------------------------FUNCTIONS-----------------------------------


