///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Health.cpp
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
#include "Health.h"
#include "Obj.h"
#include "Spaces.h"
#include "EventManager.h"
#include "Engine.h"
#include "Stats.h"
#include "Score.h"

///----------------------------------GLOBALS-----------------------------------
static bool deathinit = false;

///---------------------------------FUNCTIONS-----------------------------------

Health::Health(int maxHP) : Component(Component::Health_)
{
  maxHP_ = maxHP;
  hitPoints_ = maxHP;
}

Health::Health(const Health & ogHealth) : Component(Component::Health_)
{
  maxHP_ = ogHealth.MaxHP();
  hitPoints_ = ogHealth.HitPoints();

  IsActive(ogHealth.IsActive());
}

Health::Health(Component * health) : Component(Component::Health_)
{
  Health *hp = dynamic_cast<Health*>(health);

  maxHP_ = hp->maxHP_;
  hitPoints_ = hp->hitPoints_;
  //IsActive(health->IsActive());

}

Component * Health::Clone() const
{
  Health *hp = new Health(maxHP_);
  if (hp == NULL)
    return NULL;

  *hp = *this;

  return hp;
}

void Health::Update(float dt)
{
  if (!IsActive() || Invincible())
    return;

  if (deathinit == false)
  {

    deathinit = true;
  }

  if (hitPoints_ <= 0 && !Parent()->IsDead())
  {
    Sprite* playSprite = Parent()->Has(Sprite);
    Animation* playAnimation = Parent()->Has(Animation);
    if (Parent()->Name() != "Player")
    {
      Audio* audio = Parent()->Has(Audio);
      audio->Play(Audio::Death_);
      EventManager *man = dynamic_cast<EventManager*>(EngineGetSystem("EventManager"));
      std::string& name = Parent()->Name();

      dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->RemoveAggro();

      Parent()->Kill();
      man->SendDeactivateEvent(name);
    }
    /*if (Parent()->Has(Stats)->Class() != Playman_)
      Space::Player()->Has(Score)->AdjustKills(1);*/
    if (Parent()->ResolutionType() == Enemy_)
      if (Space::GetPlayer().Has(Score))
        Space::GetPlayer().Has(Score)->AdjustKills(1);
  }
}

int Health::HitPoints() const
{
  return hitPoints_;
}

void Health::HitPoints(int newHP)
{
  hitPoints_ = newHP;
}

int Health::MaxHP() const
{
  return maxHP_;
}

void Health::MaxHP(int newMax)
{
  hitPoints_ = newMax;

  maxHP_ = newMax;
}

bool Health::Invincible()
{
  return invincible_;
}

void Health::Invincible(bool & vince)
{
  invincible_ = vince;
}

void Health::TakeDamage(int amount) //takes positive amount
{
  if (Invincible())
    return;

  int val = -CheckHP(-amount);
  hitPoints_ -= val;
}

void Health::AdjustHP(int& amount) //takes positive and negative
{
  if (Invincible())
    return;

  int val = CheckHP(amount);
  hitPoints_ += val;
}

void Health::Reset()
{
  hitPoints_ = maxHP_;
}

int Health::CheckHP(int amount)
{
  int hp = hitPoints_;
  int val = amount;
  hp += amount;

  if (hp < 0)
    val = -hitPoints_;
  if (hitPoints_ > maxHP_)
    val = maxHP_ - hitPoints_;

  return val;
}

