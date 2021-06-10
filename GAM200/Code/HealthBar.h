///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: HealthBar.h
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: player health bar and components
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "Obj.h"
#include "ObjFactory.h"
#include "Spaces.h"
///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class HealthBar : public Obj
{
public:
  HealthBar() : Obj("HealthBar") {} ;
  virtual void Initialize();
  virtual void Update(float dt);
  virtual void Render();
  void alphaRender(Obj* obj);

  void SetKeyActive(colorFlag color, bool yup);

private:
  Player* player_;

  // healthbar sub-objects -- should be drawn recursively, but will have to be separate for now
  Obj * base_;
  Obj * glow_;
  Obj * lantern_;
  Obj * orb_;

  // key images
  Obj * key_;
  Obj * bluekey_;
  Obj * redkey_;
  Obj * purplekey_;

  // key transformations
  Transform* trans1_;
  Transform* trans2_;
  Transform* trans3_;
  Transform* trans4_;
  Vec2D vec1_ = Vec2D(-820.0f, 420.0f);
  Vec2D vec2_ = Vec2D(-810.0f, 420.0f);
  Vec2D vec3_ = Vec2D(-800.0f, 420.0f);
  Vec2D vec4_ = Vec2D(-790.0f, 420.0f);
};

///---------------------------------FUNCTIONS-----------------------------------


