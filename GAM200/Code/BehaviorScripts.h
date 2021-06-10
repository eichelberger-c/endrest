///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: BehaviorScripts.h
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
#include "Behavior.h"
#include "Timer.h"

using namespace std;
///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class BehaviorScript : public Behavior
{
public:

  virtual ~BehaviorScript() {}
  virtual void Reset() { isAggroed_ = false; }
  virtual void Start() override {}
  virtual void SpecialUpdate(float dt) override {}
  //virtual void Update(float dt) {}
  virtual void Exit() override {}
  virtual void OnCollisionWithPlayer() {}
  virtual void OnCollision(Collider& other) {}
  virtual void OnDeactivate() {}

  /*float Timer();
  void Timer(float time);
  void AdjustTimer(float amount);
  float TimerMax();
  void TimerMax(float newMax);
  void ResetTimer();*/

  Timer& Tim();
  string Name();
  void Name(string name);
  float AggroRadius();
  void AggroRadius(float radius);
  bool IsAggroed();
  void IsAggroed(bool& aggro);

private:
  /*float timer_;
  float timerMax_;*/
  Timer tim_; //Tim the Timer don't touch him
  string name_;

  float aggroRadius_;
  bool isAggroed_;
};

///---------------------------------FUNCTIONS-----------------------------------


