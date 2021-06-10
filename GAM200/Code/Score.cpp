///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Score.cpp
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
#include "Score.h"
#include "Key.h"
#include "HealthBar.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

Score::Score() : Component(Score_)
{
  points_ = 0;
  kills_ = 0;
}

Score::Score(const Score& ogScore) : Component(Score_)
{
  points_ = ogScore.points_;
  kills_ = ogScore.kills_;
}

Component * Score::Clone() const
{
  Score *clone = new Score;
  if (clone == nullptr)
    return nullptr;

  *clone = *this;

  return clone;
}

void Score::Update(float dt)
{
}

void Score::Reset()
{
  points_ = 0;
  kills_ = 0;
}

void Score::Points(int val)
{
  points_ = val;
}

int Score::Points()
{
  return points_;
}

void Score::Kills(int val)
{
  kills_ = val;
}

int Score::Kills()
{
  return kills_;
}

void Score::AdjustPoints(int amount)
{
  points_ += amount;
  
  if (points_ < 0)
    points_ = 0;
}

void Score::AdjustKills(int amount)
{
  kills_ += amount;
  if (kills_ < 0)
    kills_ = 0;
}

void Score::AdjustCatalysts(colorFlag color, int amount)
{
  catalysts_[color] += amount;
}

void Score::AdjustCatsFromEvent(CatData & data)
{
  AdjustCatalysts(data.color, data.amount);
  TraceMessage(TraceLog, "Catalysts - Green: %d, Blue: %d, Purple: %d, Red: %d", catalysts_[Green_], catalysts_[Blue_], catalysts_[Purple_], catalysts_[Red_]);
}

void Score::IncreaseCatsBy1(colorFlag& color)
{
  catalysts_[color] += 1;
  TraceMessage(TraceLog, "Catalysts - Green: %d, Blue: %d, Purple: %d, Red: %d", catalysts_[Green_], catalysts_[Blue_], catalysts_[Purple_], catalysts_[Red_]);
}

void Score::DecreaseCatsBy1(colorFlag& color)
{
  catalysts_[color] -= 1;  TraceMessage(TraceLog, "Catalysts - Green: %d, Blue: %d, Purple: %d, Red: %d", catalysts_[Green_], catalysts_[Blue_], catalysts_[Purple_], catalysts_[Red_]);}

///------------------------------------- 
/// Brief: adds item to inventory 
void Score::AddItem(Obj * obj)
{
  inventory_.push_back(obj);
}

///------------------------------------- 
/// Brief: removes item from inventory
void Score::RemoveItem(Obj * obj)
{
  std::vector<Obj*>::iterator it = std::find(inventory_.begin(), inventory_.end(), obj);
  inventory_.erase(it);

  if (obj->Type() == "Key")
  {
    if (!HasItem("Key", obj->Color()))
      static_cast<HealthBar*>(Parent()->GetSubObject("HealthBar"))->SetKeyActive(obj->Color(), false);
  }
}

///------------------------------------- 
/// Brief: looks for key matching door ID
///        returns nullptr if none
Obj * Score::FindKey(std::string doorID)
{
  for (Obj * obj : inventory_)
  {
    Key *key = obj->Has(Key);

    if (key == nullptr)
      continue;

    if (key->ID().compare(doorID) == 0)
      return obj;
  }

  return nullptr;
}

bool Score::HasItem(std::string name, colorFlag color)
{
  for (Obj * obj : inventory_)
  {
    if (obj->Type() == name && obj->Color() == color)
      return true;
  }

  return false;
}
