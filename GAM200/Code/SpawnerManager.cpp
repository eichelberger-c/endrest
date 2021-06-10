///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SpawnerManager.cpp
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
#include "SpawnerManager.h"
#include "Engine.h"

///----------------------------------GLOBALS-----------------------------------

SpawnerManager::SpawnerManager() : System("SpawnerManager")
{
  numSpawners_ = 0;
  numBanshee_ = 0;
  numWraith_ = 0;
  numGhoul_ = 0;
  numShadowmen_ = 0;

  maxSpawners_ = 5;
  spawnerMade_ = '~';
  baseID_ = '!';
  currID_ = baseID_;
}

int NumB()
{
  SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));

  return sM->NumBanshee();
}

int NumW()
{
  SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));

  return sM->NumWraith();
}

int NumG()
{
  SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));

  return sM->NumGhoul();
}

int NumS()
{
  SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));

  return sM->NumShadowmen();
}

void SpawnerManager::Initialize()
{
  numSpawners_ = 0;
  numBanshee_ = 0;
  numWraith_ = 0;
  numGhoul_ = 0;
  numShadowmen_ = 0;

  maxSpawners_ = 6;

  SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));

  spawnerNumLookup_[Banshee_] = &NumB;
  spawnerNumLookup_[Wraith_] = &NumW;
  spawnerNumLookup_[Ghoul_] = &NumG;
  spawnerNumLookup_[Shadowman_] = &NumS;
}

void SpawnerManager::Update(float dt)
{
}

int SpawnerManager::GetNumofType(eClass clss)
{
  return spawnerNumLookup_[clss]();
}

int SpawnerManager::GetNumofType(std::string type)
{
  eClass e = Stats::StringToClass(type);

  return spawnerNumLookup_[e]();
}

void SpawnerManager::AddSpawner(Spawner * spawner)
{
  SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));
  spawner->ID(sM->GetNextID());
  sM->spawners_.push_back(spawner);
  //sM->IncrementEs(spawner->EnemyType()); //dont think I want this here?
}

Spawner * SpawnerManager::GetSpawner(std::string name)
{
  SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));

  int size = sM->spawners_.size();
  for (int i = 0; i < size; ++i)
    if (sM->spawners_[i]->Name() == name)
      return sM->spawners_[i];

  return nullptr;
}

Spawner * SpawnerManager::GetASpanwerofType(eClass type)
{
  SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));

  int size = sM->spawners_.size();
  for (int i = 0; i < size; ++i)
    if (sM->spawners_[i]->EnemyType() == type)
      return sM->spawners_[i];

  return nullptr;
}

void SpawnerManager::IncrementEs(eClass type)
{
  SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));

  if (type == Banshee_)
    ++(sM->numBanshee_);
  if (type == Wraith_)
    ++(sM->numWraith_);
  if (type == Ghoul_)
    ++(sM->numGhoul_);
  if (type == Shadowman_)
    ++(sM->numShadowmen_);
}

char SpawnerManager::GetNextID()
{
  char temp = currID_;
  ++currID_;

  return temp;
}

void SpawnerManager::HandleOwned(Obj * obj)
{
  int size = spawners_.size();

  for (int i = 0; i < size; ++i)
    spawners_[i]->HandleIsMine(obj);
}

///---------------------------------FUNCTIONS-----------------------------------
int SpawnerManager::NumSpawners()
{
  return numSpawners_;
}

int SpawnerManager::NumBanshee()
{
  return numBanshee_;
}

int SpawnerManager::NumWraith()
{
  return numWraith_;
}

int SpawnerManager::NumGhoul()
{
  return numGhoul_;
}

int SpawnerManager::NumShadowmen()
{
  return numShadowmen_;
}

int SpawnerManager::MaxSpawners()
{
  return maxSpawners_;
}

void SpawnerManager::MaxSpawners(int newMax)
{
  maxSpawners_ = newMax;
}

char SpawnerManager::SpawnerMade()
{
  return spawnerMade_;
}

void SpawnerManager::IncrementCurrID()
{
  ++currID_;
}

char SpawnerManager::CurrID()
{
  return currID_;
}

