///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SpawnerManager.h
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
#include "Spawner.h"
#include "System.h"

///----------------------------------GLOBALS------------------------------------
typedef int(*SpawnerNumP)(void);
using SpawnerList = std::vector<Spawner*>;

///----------------------------------CLASSES------------------------------------
class SpawnerManager : public System
{
public:
  SpawnerManager();
  virtual void Initialize();
  virtual void Update(float dt);
  int GetNumofType(eClass clss);
  int GetNumofType(std::string type);
  static void AddSpawner(Spawner *spawner);
  static Spawner * GetSpawner(std::string name);
  static Spawner * GetASpanwerofType(eClass type);
  static void IncrementEs(eClass type);
  char GetNextID();
  void HandleOwned(Obj *obj);

  int NumSpawners();
  int NumBanshee(void);
  int NumWraith();
  int NumGhoul();
  int NumShadowmen();
  int MaxSpawners();
  void MaxSpawners(int newMax);
  char SpawnerMade();
  void IncrementCurrID();
  char CurrID();


  friend class Spawner;

private:
  int numSpawners_; //# total spawners
  int numBanshee_; //# banshees total 
  int numWraith_; //# wraiths total 
  int numGhoul_;
  int numShadowmen_;

  int maxSpawners_;
  char spawnerMade_;
  char baseID_;
  char currID_;

  SpawnerList spawners_;
  std::map<eClass, SpawnerNumP/*int(SpawnerManager::*)()*/> spawnerNumLookup_;
};

///---------------------------------FUNCTIONS-----------------------------------


