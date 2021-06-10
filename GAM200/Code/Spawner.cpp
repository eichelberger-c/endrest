///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Spawner.cpp
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
#include "Spawner.h"
#include "SpawnerManager.h"
#include "Stats.h"
#include "Engine.h"
#include <string>
#include "Transform.h"
#include "Audio.h"
#include "Timer.h"
#include "ObjManager.h"
#include "Trace.h"
#include "Utils.h"
#include "Spaces.h"
#include "Matrix2D.h"
#include "Mat2D.h"

///----------------------------------GLOBALS-----------------------------------
//SpawnerManager *spawnM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));
ObjManager *objM = ObjManager::GetInstance();

float defaultRad = 60.0f;
float defaultTime = 5.0f;

int SpawnEnemy(eClass, Vec2D);
int SpawnWave(eClass, int);
int SpawnerSpawnEnemy(eClass, Vec2D, char);

int spawners = 0;

///---------------------------------FUNCTIONS-----------------------------------

Spawner::Spawner(eClass enemyType) : Obj(Stats::ClassToString(enemyType) + "Spawner" + std::to_string(spawners/*dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"))->GetNumofType(enemyType)*/))
{
  ++spawners;
  SpawnerManager *sp = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));

  enemyType_ = enemyType;
  numSpawned_ = 0;
  radius_ = defaultRad;
  spawnRate_ = 1.0f / defaultTime;
  maxSpawnable_ = 3;

  Transform *trans = new Transform(0.0f, 0.0f);
  Timer *time = new Timer(defaultTime, CountDown_, 0.0f);
  Add(trans);
  Add(time);

  sp->AddSpawner(this);
}

Spawner::Spawner(std::string type) : Obj(type + "Spawner" + std::to_string(spawners/*dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"))->GetNumofType(enemyType)*/))
{
  enemyType_ = Stats::StringToClass(type);
  numSpawned_ = 0;
  radius_ = defaultRad;
  spawnRate_ = 1.0f / defaultTime;
  maxSpawnable_ = 3;

  Transform *trans = new Transform(0.0f, 0.0f);
  Timer *time = new Timer(defaultTime, CountDown_, 0.0f);
  Add(trans);
  Add(time);

  dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"))->AddSpawner(this);
}

Spawner::Spawner(eClass enemyType, Vec2D position) : Obj(Stats::ClassToString(enemyType) + "Spawner" + std::to_string(spawners/*dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"))->GetNumofType(enemyType)*/))
{
  enemyType_ = enemyType;
  numSpawned_ = 0;
  radius_ = defaultRad;
  spawnRate_ = 1.0f / defaultTime;
  maxSpawnable_ = 3;

  Transform *trans = new Transform(position);
  Timer *time = new Timer(defaultTime, CountDown_, 0.0f);
  Add(trans);
  Add(time);

  dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"))->AddSpawner(this);
}

Spawner::Spawner(std::string type, Vec2D position) : Obj(type + "Spawner" + std::to_string(spawners/*dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"))->GetNumofType(enemyType)*/))
{
  enemyType_ = Stats::StringToClass(type);
  numSpawned_ = 0;
  radius_ = defaultRad;
  spawnRate_ = 1.0f / defaultTime;
  maxSpawnable_ = 3;

  Transform *trans = new Transform(position);
  Timer *time = new Timer(defaultTime, CountDown_, 0.0f);
  Add(trans);
  Add(time);

  dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"))->AddSpawner(this);
}

Spawner::Spawner(Spawner & ogSpawner) : Obj(Stats::ClassToString(ogSpawner.enemyType_) + "Spawner" + std::to_string(spawners/*dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"))->GetNumofType(enemyType)*/))
{
  enemyType_ = ogSpawner.enemyType_;
  numSpawned_ = 0;
  spawnRate_ = ogSpawner.spawnRate_;
  maxSpawnable_ = ogSpawner.maxSpawnable_;

  Transform *trans = new Transform(*(ogSpawner.Has(Transform)));
  Timer *time = new Timer(*(ogSpawner.Has(Timer)));
  Add(trans);
  Add(time);

  dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"))->AddSpawner(this);
}

void Spawner::Update(float dt)
{
  this->Obj::Update(dt);

  ObjectList& objs = Space::GetObjList();
  int size = objs.size();
  for (int i = 0; i < size; ++i)
    if (objs[i]->IsRespawn() && objs[i]->Type() == Stats::ClassToString(enemyType_))
    {
      Vec2D pos = Has(Transform)->Translation();
      objs[i]->Has(Transform)->Translation(RandomVec2D2(pos.X() - radius_, pos.X() + radius_, pos.Y() - radius_, pos.Y() + radius_));
      objs[i]->Reset();
      objs[i]->IsRespawn(false);
    }

  return;

  /*int result;
  Timer *tim = Has(Timer);
  if (tim->Beep())
  {
    if (numSpawned_ == maxSpawnable_)
      return;

    result = SpawnerSpawnEnemy(enemyType_, RandomVec2D(-radius_, radius_), id_);
    tim->Reset();

    if (result == 1)
      ++numSpawned_;
  }*/
}

void Spawner::EnemyType(eClass newEnemyType)
{
  enemyType_ = newEnemyType;
}

eClass Spawner::EnemyType()
{
  return enemyType_;
}

void Spawner::SpawnRate(float newRate)
{
  spawnRate_ = newRate;
  float time = 1.0f / newRate;
  Has(Timer)->MaxTime(time);
}

float Spawner::SpawnRate()
{
  return spawnRate_;
}

int Spawner::NumSpawned()
{
  return numSpawned_;
}

void Spawner::MaxSpawnable(int newMax)
{
  maxSpawnable_ = newMax;
}

int Spawner::MaxSpawnable()
{
  return maxSpawnable_;
}

void Spawner::Radius(float newRadius)
{
  radius_ = newRadius;
}

float Spawner::Radius()
{
  return radius_;
}

void Spawner::ID(char newID)
{
  id_ = newID;
}

char Spawner::ID()
{
  return id_;
}

void Spawner::HandleIsMine(Obj * obj)
{
  std::string name = obj->Name();
  if (name.find(id_) != std::string::npos)
    --numSpawned_;
}

int SpawnEnemy(eClass enemyType, Vec2D position)
{
  static int counter = 0;
  ++counter;

  /*int dead = objM->NumDeadofType(enemyType);*/
  std::string type = Stats::ClassToString(enemyType);
  Obj *obj;
  int dead = 0;
  if(counter > 6)
    dead = objM->NumDeadofType(enemyType);

  if (dead < 1)
  {
    /*TraceMessage(false, "Can't spawn new %s. All % are still alive.", type.c_str(), type.c_str());
    return 0;*/

    SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));
    Obj *arch = Space::GetArchetype(type);
    std::string name = type;
    int num = sM->GetNumofType(type);
    name += std::to_string(num + 1);
    name += sM->SpawnerMade();
    obj = new Obj(*arch, name);
    obj->Has(Transform)->Translation(position);
    ObjManagerAddObject(obj);

    TraceMessage(TraceLog, "Spawned new %s at (%f, %f) named %s", type.c_str(), position.X(), position.Y(), name.c_str());

    return 1;
  }

  obj = objM->FindFirstDead(type);
  obj->Has(Transform)->Translation(position);
  obj->Resurrect();

  TraceMessage(TraceLog, "Spawned %s at (%f, %f) named %s", type.c_str(), position.X(), position.Y(), obj->Name().c_str());

  return 1;
}

int SpawnerSpawnEnemy(eClass enemyType, Vec2D position, char spawnerID)
{
  static int counter2 = 0;
  ++counter2;
  /*int dead = objM->NumDeadofType(enemyType);*/
  std::string type = Stats::ClassToString(enemyType);
  Obj *obj;
  int dead = 0;
  if (counter2 > 5)
    dead = objM->NumDeadofType(enemyType);

  if (dead < 1)
  {
    /*TraceMessage(false, "Can't spawn new %s. All % are still alive.", type.c_str(), type.c_str());
    return 0;*/

    SpawnerManager *sM = dynamic_cast<SpawnerManager*>(EngineGetSystem("SpawnerManager"));
    Obj *arch = Space::GetArchetype(type);
    std::string name = type;
    int num = sM->GetNumofType(type);
    name += std::to_string(num + 1);
    name += spawnerID;
    name += sM->SpawnerMade();
    obj = new Obj(*arch, name);
    obj->Has(Transform)->Translation(position);
    ObjManagerAddObject(obj);

    TraceMessage(TraceLog, "Spawned new %s at (%f, %f) named %s", type.c_str(), position.X(), position.Y(), name.c_str());

    return 1;
  }

  obj = objM->FindFirstDeadofMine(enemyType, spawnerID);
  obj->Has(Transform)->Translation(position);
  obj->Resurrect();

  TraceMessage(TraceLog, "Spawned %s at (%f, %f) named %s", type.c_str(), position.X(), position.Y(), obj->Name().c_str());

  return 1;
}

  //returns number of enemies successfully spawned. 
int SpawnWave(eClass enemyType, int numEnemies)
{
  float angle = 360.0f / numEnemies;
  Vec2D player = Space::GetPlayer().Has(Transform)->Translation();
  Vec2D pos1 = player + 100.0f;
  int result = 0;
  Mat2D mat(angle, player);

  for (int i = 0; i < numEnemies; ++i)
  {
    result += SpawnEnemy(enemyType, pos1);
    pos1 *= mat;
  }

  return result;
}

  //needs tested!!
void SpawnObj(std::string type, Vec2D position)
{
  ObjManager *man = ObjManager::GetInstance();
  Obj *obj = man->GetObjCurrentScreen(type);
  if (obj == nullptr)
    return;

  obj->Has(Transform)->Translation(position);
  obj->Resurrect();
}

void SpawnObj(std::string name, float x, float y)
{
  ObjManager *man = ObjManager::GetInstance();
  Obj *obj = man->GetObjCurrScreenByName(name);
  if (obj == nullptr)
    return;

  obj->Has(Transform)->Translation(Vec2D(x, y));
  obj->Resurrect();
}

void SpawnFromEvent(SpawnData & data)
{
  if (data.byName)
    SpawnObj(data.type, data.position.X(), data.position.Y());
  else
    SpawnObj(data.type, data.position);
}