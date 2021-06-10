///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: CompCreator.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Constructs each type of component for all-in-one-place access for
//         serialization.
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include "Component.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "Animation.h"
#include "Audio.h"
#include "Behavior.h"
#include "Collider.h"
#include "Obj.h"
#include "Health.h"
#include "Timer.h"
#include "Stats.h"
#include "Score.h"
#include "Key.h"
#include "ParticleEmitter.h"
#include "Spawn.h"
#include "Aggro.h"
#include "FlashSprite.h"
#include "SetVisible.h"
#include "SetDoor.h"
#include "Debug.h"
#include "SetInvincible.h"
#include "SetActive.h"
#include "ChangeMusic.h"
#include "Inspection.h"
#include "ToggleDark.h"
///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class CompCreator
{
public:
  CompCreator();
  Animation * ConstructAnimation(const char * name, unsigned int beg, unsigned int end, float dur, bool animate);
  Audio * ConstructAudio();
  Behavior * ConstructBehavior(std::string name);
  ColliderCircle * ConstructCircleCollider(float radius, bool dynamic, bool block);
  ColliderSquare * ConstructSquareCollider(float x, float y, bool dynamic, bool block);
  Health * ConstructHealth(int maxHP);
  Physics * ConstructPhysics();
  Sprite * ConstructSprite(const char *name, const char * maskName, glm::uvec2 dimensions);
  Transform * ConstructTransform(float x, float y);
  Timer * ConstrustTimer(secs max, tType type = CountDown_, secs resetVal = 0.0f);
  Stats * ConstrustStats(std::string type, int attkPower = 1);
  Score * ConstructScore();
  Key * ConstructKey(std::string keyID);
  Debug* ConstructDebug();
  ParticleEmitter * ConstructParticleEmitter(unsigned maxParticles, secs time, int radius, float size, glm::vec4 color, EmitterType eType, EmitterBehavior ebehavior);
  Spawn* ConstructSpawn(trigType type, SpawnData data);
  FlashSprite* ConstructFlashSprite(trigType type, FlashData data);
  Aggro* ConstructAggro(trigType type, BoolEventData data);
  SetDoor* ConstructSetDoor(trigType type, DoorData data);
  SetVisible* ConstructSetVisible(trigType type, BoolEventData data);
  SetInvincible* ConstructSetInvincible(trigType tpye, BoolEventData data);
  SetActive* ConstructSetActive(trigType type, BoolEventData data);
  Inspection* ConstructInspection(trigType type, PopupParam data);
  ChangeMusic* ConstructChangeMusic(trigType type, SoundData& data);
  ToggleDark * ConstructToggleDark(trigType type);

  Component::compType GetComponentType(std::string name);

  /*virtual Component * Create();
  virtual ~CompCreator() {};*/

private:

};


///---------------------------------FUNCTIONS-----------------------------------


