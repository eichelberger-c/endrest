///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Stats.cpp
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
#include "Stats.h"

///----------------------------------GLOBALS-----------------------------------
std::map<std::string, eClass> Stats::stringConvert_;
std::map<eClass, std::string> Stats::typeConvert_;

///---------------------------------FUNCTIONS-----------------------------------

Stats::Stats(std::string type, int attkPower) : Component(Stats_)
{
  class_ = StringToClass(type);
  attack_ = attkPower;
}

Stats::Stats(const Stats& ogStats) : Component(Stats_)
{
  class_ = ogStats.class_;
  attack_ = ogStats.attack_;
}

Component * Stats::Clone() const
{
  Stats *clone = new Stats(ClassToString(class_), attack_);
  if (clone == nullptr)
    return nullptr;

  /**clone = *this;*/


  return clone;
}

void Stats::Update(float dt)
{
}

eClass Stats::Class()
{
  return class_;
}

void Stats::Attack(int newAttk)
{
  attack_ = newAttk;
}

int Stats::Attack()
{
  return attack_;
}

  //Init the map of strings and enums to make converting between them easy
void Stats::StatsInit()
{
  typeConvert_[Banshee_] = "Banshee";
  typeConvert_[Wraith_] = "Wraith";
  typeConvert_[Ghoul_] = "Ghoul";
  typeConvert_[Shadowman_] = "Shadowman";
  typeConvert_[Necromancer_] = "Necromancer";

  for (int i = 0; i < eMax_; ++i)
  {
    eClass e = (eClass)i;
    std::string str = typeConvert_[e];
    stringConvert_[str] = e;
  }
}

std::string Stats::ClassToString(eClass clss) 
{
  return typeConvert_[clss];
}

eClass Stats::StringToClass(std::string type) 
{
  return stringConvert_[type];
}

void Stats::Class(eClass newClass)
{
  class_ = newClass;
}

//std::string ClassToStringType(eClass clss)
//{
//  std::string str;
//
//  switch (clss)
//  {
//  case Banshee_:
//    str = "Banshee";
//  case Wraith_:
//    str = "Wraith"
//  }
//}
//
//eClass StringTypeToClass(std::string type)
//{
//  return eClass();
//}
