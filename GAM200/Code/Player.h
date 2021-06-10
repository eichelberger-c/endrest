///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Player.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  brief: This class is for the main character.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include "Obj.h"
#include "Component.h"
#include "PeepHole.h"
#include "Fade.h"
#include <glm/glm.hpp>

// mw - this thing has to be defined if you're going to use these last two includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class Player : public Obj
{
public:
  Player();
  virtual void Initialize();
  virtual void Update(float dt);
  //void Kill();
  inline float Speed() { return movementSpeed_; }
  inline void Speed(float speed) { movementSpeed_ = speed; }
  inline bool Dying() { return dying_; }
  inline void Dying(bool die) { dying_ = die; }
  PeepHole* GetPeepHole();
  Fade* GetFade();
private:
  float movementSpeed_;
  bool dying_ = true;

};


///---------------------------------FUNCTIONS-----------------------------------


