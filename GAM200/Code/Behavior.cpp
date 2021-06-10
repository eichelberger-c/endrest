///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Behavior.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Behavior.h"
#include "Trace.h"
#include "Obj.h"
#include "SlowChase.h"
#include "FastChase.h"
#include "MimicPlayer.h"
#include "Spaces.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

Behavior::Behavior() : Component(Behavior_)
{
  Player* player = &Space::GetPlayer();
  
  if (player)
    playerIsActive_ = player->IsActive();
}

Behavior::~Behavior()
{

}

Behavior::Behavior(const Behavior& ogBehavior) : Component(Behavior_)
{
  stateCurrent_ = ogBehavior.stateCurrent_;
  stateNext_ = ogBehavior.stateNext_;
  IsActive(ogBehavior.IsActive());
}

Behavior::Behavior(Component * behavior) : Component(Behavior_)
{
  //SlowChase *slow = dynamic_cast<SlowChase*>(behavior);
  //FastChase *fast = dynamic_cast<FastChase*>(behavior);
  //MimicPlayer *mimic = dynamic_cast<MimicPlayer*>(behavior);

  //if(slow)
}

Component * Behavior::Clone() const
{
  Behavior *behavior = new Behavior();
  if (behavior == NULL)
    return NULL;

  *behavior = *this;

  return behavior;
}

void Behavior::Update(float dt)
{
  playerIsActive_ = Space::GetPlayer().IsActive();

  if (!IsActive() || !playerIsActive_)
    return;

  if (stateCurrent_ != stateNext_)
  {
    Exit();
    stateCurrent_ = stateNext_;
    //Start();
  }
  SpecialUpdate(dt);
}

  //returns the vector to set the follower's velocity to to make then follow at that speed
Vec2D GetFollow(Vec2D follower, Vec2D target, float speed)
{
  Vec2D diff = target - follower;
  ~diff;
  diff *= speed;

  return diff;
}
