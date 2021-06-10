///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Health.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Health
//      - Health (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class Health : public Component
{
public:
  Health(int maxHP);
  Health(const Health& ogHealth);
  Health(Component *health);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset();

  void TakeDamage(int amount);
  void AdjustHP(int& amount);
  
  int HitPoints() const;
  void HitPoints(int newHP);
  int MaxHP() const;
  void MaxHP(int newMax);
  bool Invincible();
  void Invincible(bool& vince);

private:
  int CheckHP(int amount);

  int hitPoints_;
  int maxHP_;
  bool invincible_ = false;
};

///---------------------------------FUNCTIONS-----------------------------------


