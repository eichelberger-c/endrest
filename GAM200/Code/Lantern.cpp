///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Lantern.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -Lantern();
//      -virtual void Update(float dt);
//      -virtual void Initialize();
//      -virtual void Initialize();
//      -void LightCollisionRes(std::string name);
//      -glm::vec2 RotLightVec(glm::vec2 normal, float alpha);
//      -void GrowVectors();
//      -glm::vec2 rightL();
//      -glm::vec2 leftL();
//      -void Alpha(float angle);
//      -void Direction();
//      -void Length(float length);
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "ObjManager.h"
#include "Transform.h"
#include "ObjFactory.h"
#include "Spaces.h"
#include "Lantern.h"
#include "Trace.h"
///----------------------------------GLOBALS-----------------------------------
//SpriteList sprites;

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Construct lantern
Lantern::Lantern() : Obj("Lantern")
{
  //Initialize triangle
  rightL_ = { 1.0f, 1.0f, 1.0f };
  leftL_ = { 1.0f, 1.0f, 1.0f };
  direction_ = { 1.0f, 1.0f, 1.0f };
  alpha_ = 5.0f;
  length_ = 90.0f;
  Color(Yellow_);
}

///-------------------------------------
/// Brief: Initilize lantern's position to be player's
/// Author: Sonya Toft
void Lantern::Initialize()
{
  //sprites = Space::GetSpriteList();

  ResolutionType("lantern");
  player_ = &Space::GetPlayer();
  Transform *trans = player_->Has(Transform);
  Add(new Transform(*trans));
  Add(new Sprite("Data/Textures/Light Blah Test.png", ""));
  Add(new Audio()); // PLAY A SOUND -- or add

  ObjManagerAddObject(this);
  //player_->AddSubObj(this);
}

///-------------------------------------
/// Brief: update position, direction, and triangle legs
void Lantern::Update(float dt)
{
  if (!ScreensActive())
    return;

  //Player is not attacking
  if (IsActive() == false)
    return;
  //If the player does not exist then the lantern should not
  if (player_ == NULL)
    return;


  /*-------------------------------------------------------------
  //THIS BLOCK WILL REMAIN COMMENTED OUT FOR NOW BECAUSE IT WILL LIKELY GET DLETED BUT POSSIBLY NOT
  //Shot recently fired
  //if (shotTaken_ == true)
  //{
  //  coolDown_--;
  //  ammoTimer_--;
  //}
  ////Check for lantern's firing cool don\wn
  //if (coolDown_ <= 0)
  //{
  //  ready_ = true;
  //  coolDown_ = 120;
  //  shotTaken_ = false;
  //}
  ////Check to replenish ammo
  //if (ammoTimer_ <= 0)
  //{
  //  if (shots_ < 3)
  //    shots_++;

  //  ammoTimer_ = 240;
  //}
  ---------------------------------------------------------------*/


  //Get player transform to copy translation for lantern position
  Transform *trans = Has(Transform);
  Transform *playTrans = player_->Has(Transform);

  glm::vec2 temp(playTrans->Scale().X(), 100);
  //trans->Scale(temp.x, temp.y); //Why this scale?; Scale transform
              //glm::vec2 temp(-playTrans->Scale().X() / 2, 0);
  
  //Set triangle legs by rotating them by alpha_ away from direction_
  direction_ = RotLightVec(direction_, trans->Rotation());
  //rightL_ = RotLightVec(direction_, alpha_); //rotate by alpha
  //leftL_ = RotLightVec(direction_, -alpha_); //-alpha OR 360 - alpha
  //Scale legs and direction by length
  //ScaleVectors();
  trans->setDirty(true);

  Obj::Update(dt);
}

///-------------------------------------
/// Brief: handle collision resolution for main mechanic
/// 
/// Set flags and call behaviors?
void Lantern::LightCollisionRes(std::string name)
{
   
}

///-------------------------------------
/// Brief: Rotate light vectors by alpha; triangle "legs"
glm::vec3 Lantern::RotLightVec(glm::vec3 normal, float alpha)
{
  return glm::rotateZ(normal, alpha);
}

///-------------------------------------
/// Brief: Scale triangle legs
void Lantern::ScaleVectors()
{
  rightL_ *= length_;
  leftL_ *= length_;
  direction_ *= length_;
}

///-------------------------------------
/// Brief: Return left leg
glm::vec3 Lantern::RightLeg()
{
  return rightL_;
}

///-------------------------------------
/// Brief: return right leg
glm::vec3 Lantern::LeftLeg()
{
  return leftL_;
}

///-------------------------------------
/// Brief: return angle of traignle apex
void Lantern::Alpha(float angle)
{
  alpha_ = angle;
}

float Lantern::Alpha()
{
  return alpha_;
}

///-------------------------------------
/// Brief: Calculate direction vector
void Lantern::UpdateDir()
{
  Transform *trans = Has(Transform);
  //direction_ = glm::normalize(direction_);
  direction_ = *(trans->TransMatrix()) * direction_; //Multiply direction vector by transform to get position and looking direction
}

void Lantern::Kill()
{
  Hide(true);
  IsDead(true);
}

///-------------------------------------
/// Brief: Return vector light is pointing in
glm::vec3 Lantern::Direction()
{
  return direction_;
}

void Lantern::Direction(glm::vec3 newDir)
{
  direction_ = newDir;
}

///-------------------------------------
/// Brief: Return scale factor of legs
void Lantern::Length(float length)
{
  length_ = length;
}

float Lantern::Length()
{
  return length_;
}

Player* Lantern::GetPlayer()
{
  return player_;
}

void Lantern::ShotTaken()
{
  shots_--;
  ready_ = false;
  shotTaken_ = true;
}

bool Lantern::IsReady()
{
  return ready_;
}

int Lantern::CoolDown()
{
  return coolDown_;
}

int Lantern::Ammo()
{
  return shots_;
}
