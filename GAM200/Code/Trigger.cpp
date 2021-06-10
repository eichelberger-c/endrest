///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Trigger.cpp
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
#include "Trigger.h"
#include "EventComp.h"
#include "Transform.h"
#include "Collider.h"
#include "Physics.h"
#include "Sprite.h"
#include "Audio.h"
#include "EventManager.h"

///----------------------------------GLOBALS-----------------------------------
int numTriggers;

///---------------------------------FUNCTIONS-----------------------------------

Trigger::Trigger() : Obj()
{
  isTriggered_ = false;
  ++numTriggers;
  IsTrigger(true);
}

//Trigger::Trigger(Vec2D position, float colliderRadius) : Obj("Trigger" + std::to_string(numTriggers))
//{
//  isTriggered_ = false;
//  ++numTriggers;
//  Transform *trans = new Transform(position);
//  Collider *col = static_cast<Collider*>(new ColliderCircle(colliderRadius, false));
//  Physics *phys = new Physics();
//  Sprite *sprite = new Sprite("Data/Textures/EventDot1.png");
//
//  //trans->Scale(0.6, 0.6);
//
//  Add(trans);
//  Add(col);
//  Add(phys);
//  Add(sprite);
//}

Trigger::Trigger(const Obj & papaObj, std::string name) : Obj(papaObj, name)
{
  isTriggered_ = false;
  IsTrigger(true);
}

void Trigger::Update(float dt)
{
  if (isTriggered_ || !IsActive())
    return;

  //if (onTimer_)
  //{
  //  Timer *tim = Has(Timer);
  //  if (tim->Beep())
  //  {
  //    DoTheEvents();
  //    isTriggered_ = true;
  //    return;
  //  }
  //  else
  //  {
  //    tim->Update(dt);
  //    return;
  //  }
  //}

  Obj::Update(dt);

  if (res_ == No_)
    return;

  for (Component *comp : components_)
  {
    if (comp->Type() > Component::IsEvent_)
    {
      EventComp<bool> *eComp = reinterpret_cast<EventComp<bool>*>(comp);
      if (eComp->isTriggered_ == false)
        return;
    }
  }

  isTriggered_ = true;

  Audio *aud = Has(Audio);
  if (aud)
  {
    if (Name().compare("Trigger9G1") == 0 || Name().compare("Trigger14D1") == 0)
      aud->Play(Audio::Collision_);
    else
      aud->Play(Audio::Default_);
  }

  ParticleEmitter *part = Has(ParticleEmitter);
  if (part)
    part->SetParticlesVisible(false);
}

void Trigger::Reset()
{
  //Obj::ResetAll();

  isTriggered_ = false;
  for (Component *comp : components_)
    if (comp->Type() > Component::IsEvent_)
      comp->Reset();
}

void Trigger::SetTriggerType(trigType t)
{
  trig_ = t;

  for (Component *comp : components_)
  {
    if (comp->Type() > Component::IsEvent_)
    {
      EventComp<bool> *eComp = reinterpret_cast<EventComp<bool>*>(comp);
      eComp->TriggerType(t);
    }
  }
}

void Trigger::SetCollisionType(resType t)
{
  res_ = t;
  for (Component *comp : components_)
  {
    if (comp->Type() > Component::IsEvent_)
    {
      EventComp<bool> *eComp = reinterpret_cast<EventComp<bool>*>(comp);
      eComp->CollisionType(t);
    }
  }
}

void Trigger::SetRoomsNeeded(std::vector<std::string> rooms)
{
  for (Component *comp : components_)
  {
    if (comp->Type() > Component::IsEvent_)
    {
      EventComp<SpawnData> *eComp = reinterpret_cast<EventComp<SpawnData>*>(comp);
      eComp->RoomsNeeded(rooms);
    }
  }
}

void Trigger::Collide(Collider & other)
{
  if (!IsActive() || isTriggered_)
    return;

  for (Component *comp : components_)
  {
    if (comp->Type() > Component::IsEvent_)
    {
      comp->OnCollision(other);
    }
  }
}

void Trigger::Collide(resType type)
{
  if (!IsActive() || isTriggered_)
    return;

  for (Component *comp : components_)
  {
    if (comp->Type() > Component::IsEvent_)
    {
      comp->OnCollision(type);
    }
  }
}

trigType Trigger::GetTriggerType()
{
  return trig_;
}

resType Trigger::GetCollisionType()
{
  return res_;
}

std::vector<std::string> Trigger::GetRoomsNeeded()
{
  return rooms_;
}

void Trigger::TriggerType(trigType type)
{
  trig_ = type;
}

void Trigger::CollisionType(resType type)
{
  res_ = type;
}

void Trigger::Rooms(std::vector<std::string> rooms)
{
  rooms_ = rooms;
}

void Trigger::DoTheEvents()
{
  for (Component *comp : components_)
  {
    if (comp->Type() > Component::IsEvent_)
    {
      comp->DoTheEvent();
    }
  }
}
