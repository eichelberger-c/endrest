///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Behavior.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Behavior
//      - Behavior (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "stdafx.h"
#include "Component.h"
#include "Vec2D.h"
//#include "BehaviorScript.h"

///----------------------------------GLOBALS------------------------------------
//typedef BehaviorScript BehaviorScript;

///----------------------------------CLASSES------------------------------------

class Behavior : public Component
{
public:
  Behavior();
  virtual ~Behavior();
  Behavior(const Behavior& ogBehavior);
  Behavior(Component *behavior);
  virtual Component * Clone() const;
  void Update(float dt);
  virtual void Reset() {}

  virtual void Start() {}
  virtual void SpecialUpdate(float dt) {}
  virtual void Exit() {}

  int StateCurrent() { return stateCurrent_; }
  int StateNext() { return stateNext_; }
  void StateCurrent(int state) { stateCurrent_ = state; }
  void StateNext(int state) 
  { 
    stateNext_ = state; 
  }
  Obj * GameObject() { return Parent(); }

private:

  int stateCurrent_;
  int stateNext_;
  bool playerIsActive_;
};

///---------------------------------FUNCTIONS-----------------------------------
Vec2D GetFollow(Vec2D follower, Vec2D target, float speed);

