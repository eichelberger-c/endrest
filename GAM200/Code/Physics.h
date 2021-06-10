///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Physics.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//    - Physics();
//		- Physics(const Physics& ogPhysics);
//		- virtual void Update(float dt);
//		- Vec2D OldPosition();
//		- Vec2D Velocity();
//		- float Acceleration();
//		- void SetPosition(Vec2D position);
//		- void SetVelocity(Vec2D *velocity);
//		- void MultAcceleration(float multiplier);
//		- void SetAcceleration(float acceleration);
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"
#include "Vec2D.h"
#include <glm\glm.hpp>

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class Physics : public Component
{
public:
  Physics();
  Physics(Vec2D velocity, float acceleration);
  Physics(float velX, float velY, float accel);
  Physics(const Physics& ogPhysics);
  Physics(Component *phys);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset();// check me

  Vec2D OldPosition();

  Vec2D Velocity();
  glm::vec2 GlmVelocity();
  float Acceleration();
  void Position(Vec2D position);
  void Velocity(Vec2D velocity);
  void Velocity(float X, float Y);
  void Velocity(glm::vec2 velocity);
  void MultAcceleration(float multiplier);
  void Acceleration(float acceleration);

private:
  Vec2D oldPosition_;
  Vec2D velocity_;
  float acceleration_;
};

///---------------------------------FUNCTIONS-----------------------------------

