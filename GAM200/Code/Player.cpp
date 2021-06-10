///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Player.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -Player
//      -~Player
//      - Initialize
//      - Update
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "ObjManager.h"
#include "Transform.h"
#include "ObjFactory.h"
#include "Spaces.h"
#include "Animation.h"
#include "Player.h"
#include "Trace.h"
#include "spawner.h"
#include "Engine.h"
#include "Lighting.h"
#include "HealthBar.h"
#include "ParticleEmitter.h"
#include "Engine.h"
#include "TextureSys.h"
#include "Fade.h"
///----------------------------------GLOBALS-----------------------------------
//SpriteList sprites;
static SpriteSource* idle;
static SpriteSource* run;
static SpriteSource* death;

static ObjManager objMan;
///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Construct player
Player::Player() : Obj("Player")
{
}

///-------------------------------------
/// Brief: Initilize player
void Player::Initialize()
{
  TextureSys* textSys = reinterpret_cast<TextureSys*>(EngineGetSystem("TextureSys"));
  ResolutionType("player");
  PeepHole* hole = new PeepHole();
  Fade* fade = new Fade();
  HealthBar *HB = new HealthBar();
  Lantern *lantern = new Lantern();

  Has(Audio)->AddSound(Audio::Damage_, "Data/Audio/PlayerDamage.wav");

  // init
  HB->Initialize();
  lantern->Initialize();
  fade->Initialize();
  hole->Initialize();

  // add
  AddSubObj(hole);
  AddSubObj(fade);
  AddSubObj(HB);
  AddSubObj(lantern);


  movementSpeed_ = 300;
  //Has(Animation)->SetAnimating(true);
  Add(new Lighting("Light"));
  // keep this here, not in constructor
  idle = textSys->FindSource(std::string("Data/Textures/ProtagIdleAnimation.png"), 7, 7);
  run = textSys->FindSource(std::string("Data/Textures/ProtagRunAnimation.png"), 7, 7);
  death = textSys->FindSource(std::string("Data/Textures/ProtagDeathAnimation.png"), 7, 7);
}

///-------------------------------------
/// Brief: update player
void Player::Update(float dt)
{
  if(!IsActive())
    return;
  Health* health = Has(Health);
  if (health)
  {
    Physics* phys = Has(Physics);
    Sprite* playSprite = Has(Sprite);
    Animation* playAnimation = Has(Animation);
    Transform* trans = Has(Transform);
    Vec2D Velocity = phys->Velocity();
    Vec2D scale = trans->Scale();
    if (health->HitPoints() > 0)
    {
      if (Velocity.X() != 0 || Velocity.Y() != 0)
      {
        playSprite->SetSpriteSource(run);
        if ((scale.X() > 0 && Velocity.X() < 50) || (scale.X() < 0 && Velocity.X() > 50))
          playAnimation->SetDirection(Forward_);
        else
          playAnimation->SetDirection(Reverse_);
      }
      else
      {
        playSprite->SetSpriteSource(idle);
      }
      Obj::Update(dt);
    }
    else
    {
      IsDead(false);
      bool t = true;
      IsActive(t);
      playSprite->SetSpriteSource(death);
      playAnimation->SetBegFrame(0);
      playAnimation->SetEndFrame(34);
      playAnimation->SetDirection(Death_);
      playAnimation->SetFrameDur(60.f);
      Has(Animation)->Update(dt);

    }
  }
}

PeepHole* Player::GetPeepHole()
{
  std::vector<Obj*> subList = GetSubObjList();
  int size = subList.size();
  for (int i = 0; i < size; ++i)
  {
    if (subList[i]->Name() == "PeepHole")
      return reinterpret_cast<PeepHole*>(subList[i]);
  }
  return nullptr;
}

Fade* Player::GetFade()
{
  std::vector<Obj*> subList = GetSubObjList();
  int size = subList.size();
  for (int i = 0; i < size; ++i)
  {
    if (subList[i]->Name() == "Fade")
      return reinterpret_cast<Fade*>(subList[i]);
  }
  return nullptr;
}