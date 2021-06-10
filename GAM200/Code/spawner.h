///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Spawner.h
//
//  Author: Sonya Toft
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Brief: Spawner class. Currently only act as respawn locations for already 
//         existing enemies. 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include "Obj.h"
#include "Timer.h"
#include "Stats.h"
#include "Vec2D.h"

///----------------------------------GLOBALS------------------------------------
//typedef SpawnerManager SpawnerManager;

///----------------------------------CLASSES------------------------------------
struct SpawnData
{
  SpawnData(Vec2D pos = { 0.0f, 0.0f }, std::string typ = "Banshee", bool byN = false) : position(pos), type(typ), byName(byN)
  {
  }

  Vec2D position;
  std::string type;
  bool byName;
};

class Spawner : public Obj
{
public:
  Spawner(eClass enemyType);
  Spawner(std::string type);
  Spawner(eClass enemyType, Vec2D position);
  Spawner(std::string type, Vec2D position);
  Spawner(Spawner& ogSpawner);
  virtual void Update(float dt);

  void EnemyType(eClass newEnemyType);
  eClass EnemyType();
  void SpawnRate(float newRate);
  float SpawnRate();
  int NumSpawned();
  void MaxSpawnable(int newMax);
  int MaxSpawnable();
  void Radius(float newRadius);
  float Radius();
  void ID(char newID);
  char ID();
  void HandleIsMine(Obj *obj);

  friend class SpawnerManager;

private:
  //Timer timer_;
  eClass enemyType_;
  float spawnRate_;
  int numSpawned_;
  int maxSpawnable_;
  float radius_;
  char id_;

  //static int numSpawners;
};

///---------------------------------FUNCTIONS-----------------------------------
int SpawnEnemy(eClass enemyType, Vec2D position);
int SpawnWave(eClass enemyType, int numEnemies);
void SpawnObj(std::string type, Vec2D position);
void SpawnObj(std::string name, float x, float y);
void SpawnFromEvent(SpawnData& data);

  