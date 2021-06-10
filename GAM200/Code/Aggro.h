///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Aggro.h
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
#include "EventComp.h"
#include "Spawner.h"
#include "Engine.h"
#include "Utils.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class Aggro : public EventComp<bool>
{
public:
  Aggro(trigType type, BoolEventData& data);
  Aggro(BoolEventData& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  Aggro(BoolEventData& data, resType collisionT = Player_, trigType type = OnCollision_);
  Aggro(Aggro& ogAggro);

  void SetData(BoolEventData data);
  void EnemyName(std::string name);
  std::string EnemyName();
  bool Bool();
  virtual void ReGetEvent();

private:
  std::string enemyName_;
};

///---------------------------------FUNCTIONS-----------------------------------


