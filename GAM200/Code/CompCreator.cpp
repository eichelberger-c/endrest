///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: CompCreator.cpp
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
#include "Door.h"
#include "SlowChase.h"
#include "FastChase.h"
#include "MimicPlayer.h"
#include "CompCreator.h"
#include "Collect.h"
#include "Necromancer.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

CompCreator::CompCreator()
{

}

Animation * CompCreator::ConstructAnimation(const char * name, unsigned int beg, unsigned int end, float dur, bool animate)
{
  return new Animation(name, beg, end, dur, animate);
}

Audio * CompCreator::ConstructAudio()
{
  return new Audio();
}

//BEHAV
Behavior * CompCreator::ConstructBehavior(std::string name)
{
  if (name.compare("SlowChase") == 0)
    return dynamic_cast<Behavior*>(new SlowChase());
  else if (name.compare("FastChase") == 0)
    return dynamic_cast<Behavior*>(new FastChase());
  else if (name.compare("MimicPlayer") == 0)
    return dynamic_cast<Behavior*>(new MimicPlayer());
  else if (name.compare("Collect") == 0)
    return dynamic_cast<Behavior*>(new Collect());
  else if (name.compare("Door") == 0)
    return dynamic_cast<Behavior*>(new Door());
  else if (name.compare("Necromancer") == 0)
    return dynamic_cast<Necromancer*>(new Necromancer());

  return nullptr;
}

ColliderCircle * CompCreator::ConstructCircleCollider(float radius, bool dynamic, bool block)
{
  return new ColliderCircle(radius, dynamic, block);
}

ColliderSquare * CompCreator::ConstructSquareCollider(float x, float y, bool dynamic, bool block)
{
  return new ColliderSquare(x, y, dynamic, block);
}

Health * CompCreator::ConstructHealth(int maxHP)
{
  return new Health(maxHP);
}

Physics * CompCreator::ConstructPhysics()
{
  return new Physics();
}

Sprite * CompCreator::ConstructSprite(const char *name, const char * maskName, glm::uvec2 dimensions)
{
  return new Sprite(name, maskName, dimensions);
}

Transform * CompCreator::ConstructTransform(float x, float y)
{
  return new Transform(x, y);
}

Timer * CompCreator::ConstrustTimer(secs max, tType type, secs resetVal)
{
  return new Timer(max, type, resetVal);
}

Stats * CompCreator::ConstrustStats(std::string type, int attkPower)
{
  return new Stats(type, attkPower);
}

Score * CompCreator::ConstructScore()
{
  return new Score;
}

Key * CompCreator::ConstructKey(std::string keyID)
{
  return new Key(keyID);
}

Debug * CompCreator::ConstructDebug()
{
  return new Debug();
}

ParticleEmitter * CompCreator::ConstructParticleEmitter(unsigned maxParticles, secs time, int radius, float size, glm::vec4 color, EmitterType eType, EmitterBehavior ebehavior)
{
  return new ParticleEmitter(maxParticles, time, radius, size, color, eType, ebehavior);
}

Spawn * CompCreator::ConstructSpawn(trigType type, SpawnData data)
{
	return new Spawn(type, data);
}

FlashSprite * CompCreator::ConstructFlashSprite(trigType type, FlashData data)
{
	return new FlashSprite(type, data);
}

Aggro * CompCreator::ConstructAggro(trigType type, BoolEventData data)
{
	return new Aggro(type, data);
}

SetDoor * CompCreator::ConstructSetDoor(trigType type, DoorData data)
{
	return new SetDoor(type, data);
}

SetVisible * CompCreator::ConstructSetVisible(trigType type, BoolEventData data)
{
	return new SetVisible(type, data);
}

SetInvincible * CompCreator::ConstructSetInvincible(trigType type, BoolEventData data)
{
	return new SetInvincible(type, data);
}

SetActive * CompCreator::ConstructSetActive(trigType type, BoolEventData data)
{
	return new SetActive(type, data);
}

Inspection * CompCreator::ConstructInspection(trigType type, PopupParam data)
{
	return new Inspection(type, data);
}

ChangeMusic * CompCreator::ConstructChangeMusic(trigType type, SoundData & data)
{
	return new ChangeMusic(type, data);
}

ToggleDark * CompCreator::ConstructToggleDark(trigType type)
{
  bool decoy = true;

  return new ToggleDark(type, decoy);
}

//COMP
Component::compType CompCreator::GetComponentType(std::string name)
{
  Component::compType type = Component::Invalid_;

  if (name.compare("Audio") == 0)
    type = Component::Audio_;
  else if (name.compare("Animation") == 0)
    type = Component::Animation_;
  else if (name.compare("Behavior") == 0)
    type = Component::Behavior_;
  else if (name.compare("Collider") == 0)
    type = Component::Collider_;
  else if (name.compare("Health") == 0)
    type = Component::Health_;
  else if (name.compare("ParticleEmitter") == 0)
    type = Component::ParticleEmitter_;
  else if (name.compare("Physics") == 0)
    type = Component::Physics_;
  else if (name.compare("Key") == 0)
    type = Component::Key_;
  else if (name.compare("ObjInput") == 0)
    type = Component::ObjInput_;
  else if (name.compare("Sprite") == 0)
    type = Component::Sprite_;
  else if (name.compare("Debug") == 0)
    type = Component::Debug_;
  else if (name.compare("Transform") == 0)
    type = Component::Transform_;
  else if (name.compare("Color") == 0)
    type = Component::Color_;
  else if (name.compare("Stats") == 0)
    type = Component::Stats_;
  else if (name.compare("Timer") == 0)
    type = Component::Timer_;
  else if (name.compare("Score") == 0)
    type = Component::Score_;
  else if (name.compare("ResType") == 0)
    type = Component::ResType_;
  else if (name.compare("Spawn") == 0)
    type = Component::Spawn_;
  else if (name.compare("Aggro") == 0)
    type = Component::Aggro_;
  else if (name.compare("SetVisible") == 0)
    type = Component::SetVisible_;
  else if (name.compare("SetActive") == 0)
    type = Component::SetActive_;
  else if (name.compare("FlashSprite") == 0)
    type = Component::FlashSprite_;
  else if (name.compare("SetDoor") == 0)
    type = Component::SetDoor_;
  else if (name.compare("SetInvincible") == 0)
    type = Component::SetInvincible_;
  else if (name.compare("Inspection") == 0)
    type = Component::Inspection_;
  else if (name.compare("ChangeMusic") == 0)
    type = Component::ChangeMusic_;
  else if (name.compare("ToggleDark") == 0)
    type = Component::ToggleDark_;

  return type;
}


