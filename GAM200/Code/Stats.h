///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Stats.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Stats
//      - Stats (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"
#include "Events.h"
#include "Utils.h"

///----------------------------------GLOBALS------------------------------------
typedef enum EnemyClasses
{
  Banshee_,
  Wraith_,
  Ghoul_,
  Shadowman_,
  Necromancer_,
  NotEnemy_,
  Playman_,
  eMax_,

} eClass;

///----------------------------------CLASSES------------------------------------

class Stats : public Component
{
public:
  Stats(std::string type, int attkPower = 1);
  Stats(const Stats& ogStats);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset() {}

  eClass Class();
  void Attack(int newAttk);
  int Attack();

  static void StatsInit();
  static std::string ClassToString(eClass clss);
  static eClass StringToClass(std::string type);

  EventZone::Event<int> light;

private:
  void Class(eClass newClass);

  eClass class_;
  int attack_;

  static std::map<std::string, eClass> stringConvert_;
  static std::map<eClass, std::string> typeConvert_;
};

///---------------------------------FUNCTIONS-----------------------------------
//std::string ClassToStringType(eClass clss);
//eClass StringTypeToClass(std::string type);

