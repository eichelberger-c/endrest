///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Lantern.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  brief: This class is for the main mechanic.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include "Obj.h"
#include "Component.h"
#include "Player.h"
#include <glm/glm.hpp>

// mw - this thing has to be defined if you're going to use these last two includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/projection.hpp>

///----------------------------------GLOBALS------------------------------------
//typedef enum LightColors
//{
//  NoColor_ = -1,
//  Yellow_, //neutral
//
//  Green_,
//  //Blue_,
//  //Purple_,
//  //Red_,
//
//} colorFlag;

///----------------------------------CLASSES------------------------------------
class Lantern : public Obj
{
public:
  Lantern();
  virtual void Initialize();
  virtual void Update(float dt);
  void UpdateDir();
  void Kill();

  void LightCollisionRes(std::string name);
  glm::vec3 RotLightVec(glm::vec3 normal, float alpha);

  void ScaleVectors();
  glm::vec3 RightLeg();
  glm::vec3 LeftLeg();
  void Alpha(float angle);
  float Alpha();
  glm::vec3 Direction();
  void Direction(glm::vec3 newDir);
  void Length(float length);
  float Length();
  Player* GetPlayer();
  void ShotTaken();
  bool IsReady();
  int CoolDown();
  int Ammo();


private:
  Player* player_;         //
  float alpha_;         //Angle between direction_ and A1
  glm::vec3 rightL_;    //Leg 1
  glm::vec3 leftL_;     //Leg 2
  glm::vec3 direction_; //"looking" vector; should be same length as legs, for projection
  float length_;        //Scale of triangle legs
  int shots_ = 3;
  int coolDown_ = 120;
  int ammoTimer_ = 240;
  bool ready_ = true;
  bool shotTaken_ = false;

};


///---------------------------------FUNCTIONS-----------------------------------


