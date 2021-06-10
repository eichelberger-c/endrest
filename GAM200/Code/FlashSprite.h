///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: FlashSprite.h
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
#include "Utils.h"
#include "Engine.h"
#include "Timer.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

//For this one, the obj that you want to flash must already exist invisibly in the location you want it at
class FlashSprite : public EventComp<bool>
{
public:
  FlashSprite(trigType type, FlashData& data);
  FlashSprite(FlashData& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  FlashSprite(FlashData& data, resType collisionT = Player_, trigType type = OnCollision_);
  FlashSprite(FlashSprite& ogFlashSprite);
  ~FlashSprite();

  virtual void Update(float dt) override;

  void Flash(bool& decoy);
  void SetData(FlashData& data);
  std::string ObjName();
  secs Duration();

private:
  Timer tom_;
  bool flash_ = false;
  FlashData fd_;

  EventZone::Event<bool> ev_;
};

///---------------------------------FUNCTIONS-----------------------------------


