///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Aggro.h
//
//  Author: Sonya Toft
//
//  Copyright © 2018 DigiPen (USA) Corporation
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
  Aggro(trigType type, AggroData& data);
  Aggro(AggroData& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  Aggro(AggroData& data, resType collisionT = Player_, trigType type = OnCollision_);
  Aggro(Aggro& ogAggro);

private:

};

///---------------------------------FUNCTIONS-----------------------------------


