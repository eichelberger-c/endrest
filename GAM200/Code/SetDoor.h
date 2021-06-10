///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SetDoor.h
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

///----------------------------------GLOBALS------------------------------------

  //1 = Open
  //2 = Locked
  //3 = Unlocked
struct DoorData
{
  DoorData(int state = 3, std::string ID = "TEST", std::string screenName = "EntranceToEndrest") : newState(state), doorID(ID), screen(screenName)
  {
  }

  int newState;
  std::string doorID;
  std::string screen;
};

///----------------------------------CLASSES------------------------------------
class SetDoor : public EventComp<DoorData>
{
public:
  SetDoor(trigType type, DoorData& data);
  SetDoor(DoorData& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  SetDoor(DoorData& data, resType collisionT = Player_, trigType type = OnCollision_);
  SetDoor(SetDoor& ogSetDoor);

  void SetData(DoorData& data);
  int NewState();
  std::string DoorID();
  std::string ScreenName();

private:

};

///---------------------------------FUNCTIONS-----------------------------------
std::string GetStringFromDoorState(int state);

