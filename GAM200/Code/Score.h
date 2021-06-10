///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Score.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Score
//      - Score (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Obj.h"
//#include "Component.h"
#include "Utils.h"

///----------------------------------GLOBALS------------------------------------

struct CatData
{
  colorFlag color;
  int amount;
};

///----------------------------------CLASSES------------------------------------

class Score : public Component
{
public:
  Score();
  Score(const Score& ogScore);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset();

  void Points(int val);
  int Points();
  void Kills(int val);
  int Kills();
  void AdjustPoints(int amount);
  void AdjustKills(int amount);
  void AdjustCatalysts(colorFlag color, int amount);
  void AdjustCatsFromEvent(CatData& data);
  void IncreaseCatsBy1(colorFlag& color);
  void DecreaseCatsBy1(colorFlag& color); //necessary? there are never enough cats 

  // inventory functions
  void AddItem(Obj* obj);
  void RemoveItem(Obj* obj);
  Obj* FindKey(std::string doorID);
  bool HasItem(std::string name, colorFlag color);

private:
  int points_;
  int kills_;
  std::map<colorFlag, int> catalysts_;
  //int catalysts_[ColorMax_];
  std::vector<Obj*> inventory_;
};

///---------------------------------FUNCTIONS-----------------------------------


