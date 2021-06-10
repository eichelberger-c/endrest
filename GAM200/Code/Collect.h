///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Collect.h
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
#include "Spaces.h"
#include "Player.h"
#include "EventManager.h"
#include "Engine.h"
#include "Key.h"
#include "HealthBar.h"

///----------------------------------GLOBALS------------------------------------
typedef enum CollectStates
{
  coInvalid,
  coCollect,

} coState;

///----------------------------------CLASSES------------------------------------
class Collect : public BehaviorScript
{
public:

  Collect(std::string name = "Collect"); //always use the default DO NOT CHANGE THE NAME
  Collect(Collect& ogCollect);
  Player *player;

  virtual void Start() override
  {
    Player& p = Space::GetPlayer();
    player = &p;
  }

  virtual void SpecialUpdate(float dt) override
  {
    coState state = (coState)StateCurrent();
    switch (state)
    {
    case coInvalid:
      break;
    case coCollect:
      break;
    }
  }

  virtual void Exit() override
  {

  }

  virtual void OnCollisionWithPlayer() override
  {
    EventManager *man = dynamic_cast<EventManager*>(EngineGetSystem("EventManager"));

    if (Parent()->ResolutionType() == Catalyst_)
    {
      colorFlag color = Parent()->Color();
      man->catPlus(color);
      Parent()->Kill();
    }
    else
    {
      //add key/item logic here
      Key* key = dynamic_cast<Key*>(Parent()->Has(Key));
      player->Has(Score)->AddItem(Parent());
      Parent()->Kill();

      static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/Key.wav");
      static_cast<HealthBar*>(player->GetSubObject("HealthBar"))->SetKeyActive(Parent()->Color(), true);
    }
  }

  virtual void OnCollision(Collider& other) override
  {

  }
};

///---------------------------------FUNCTIONS-----------------------------------


