///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: MimicPlayer.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: A behavior "script" that //fill this in 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "BehaviorScripts.h"
#include "Vec2D.h"
#include "Obj.h"
#include "Physics.h"
#include "Spaces.h"

///----------------------------------GLOBALS------------------------------------
typedef enum MimicPlayerStates
{
  ///REQUIRED:
  mpInvalid,
  mpMimic,

} mpState;

///----------------------------------CLASSES------------------------------------
class MimicPlayer : public BehaviorScript
{
public:

  MimicPlayer(std::string name = "MimicPlayer"); //always use the default DO NOT CHANGE THE NAME
  MimicPlayer(MimicPlayer& ogMimicPlayer);

  Vec2D playerVel = { 0.0f };
  float lag;

  virtual void Start() override
  {
    StateCurrent(mpInvalid);
    StateNext(mpMimic);
    lag = 0.1f;
  }

  virtual void SpecialUpdate(float dt) override
  {
		ColliderCircle *collider = Space::GetCircleCollider(Parent()->Name());
		if (collider->Collided())
			return;

    mpState state = (mpState)StateCurrent();

    switch (state)
    {
    case mpInvalid:
      break;
    case mpMimic:
      playerVel = Space::GetPlayer().Has(Physics)->Velocity();
      //if(playerVel > 0.0f)
      //  Parent()->Has(Physics)->Velocity(playerVel - lag);
      //else if(playerVel < 0.0f)
      //  Parent()->Has(Physics)->Velocity(playerVel - lag);
      //else
      //  Parent()->Has(Physics)->Velocity(playerVel);

      Parent()->Has(Physics)->Velocity(playerVel - (playerVel * lag));
    }
  }

  virtual void Exit() override
  {

  }

  virtual void OnCollisionWithPlayer() override
  {

  }

  virtual void OnCollision(Collider& other) override
  {

  }

  virtual void OnDeactivate() override
  {

  }

  void Lag(float val);
};

///---------------------------------FUNCTIONS-----------------------------------


