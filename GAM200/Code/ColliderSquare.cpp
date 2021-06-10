///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ColliderSquare.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -ColliderSquare(float dimensionX, float dimensionY, bool dynamic)
//		-ColliderSquare(const Component& ogComponent)
//		-virtual Component * Clone() const
//		-virtual void Update(float dt)
//		-void SquareSquareResolution(int zone, bool isDynamic, std::string firstName, std::string secondName)
//		-bool Dynamic()
//		-Vec2D Dimensions()
//		-float Speed()
//		-void Collided()
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "ColliderSquare.h"
#include "Spaces.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------


///-------------------------------------
/// Brief: Initialize square collider
ColliderSquare::ColliderSquare(float dimensionX, float dimensionY, bool dynamic, bool block)
{
  dimensions_.Set(dimensionX, dimensionY);
  isDynamic_ = dynamic;
  speed_ = 1.5f;
  collided_ = false;
  frames_ = 0;
  pushFrames_ = 0;
  isBlock_ = block;
}

///-------------------------------------
/// Brief: 
ColliderSquare::ColliderSquare(const ColliderSquare &ogComponent)
{
  dimensions_ = ogComponent.dimensions_;
  isDynamic_ = ogComponent.isDynamic_;
  speed_ = ogComponent.speed_;
  collided_ = false;
  frames_ = 0;
  pushFrames_ = 0;
  isBlock_ = ogComponent.isBlock_;
}

ColliderSquare::ColliderSquare(ColliderSquare *square)
{
  dimensions_ = square->Dimensions();
  isDynamic_ = square->Dynamic();
  speed_ = square->Speed();
  collided_ = false;
  frames_ = 0;
  pushFrames_ = 0;
  isBlock_ = square->isBlock_;
}

///-------------------------------------
/// Brief: 
Component * ColliderSquare::Clone() const
{
  ColliderSquare *square = new ColliderSquare(*this);

  if (square == nullptr)
    return nullptr;
  else
    return square;
}

///-------------------------------------
/// Brief: Update collider's collision resolution
///			effects.
void ColliderSquare::Update(float dt)  //This may need to change; pushing?
{
  if (!IsActive())
    return;

  if (!isDynamic_)
    return;

  Obj *parent = Parent();
  std::string parent_ = parent->Name();

  if (collided_ == false)
  {
    Physics *physics = Space::GetPhysics(parent_);
    physics->Velocity(0.0f, 0.0f);
  }
}

void ColliderSquare::Reset()
{
  collided_ = false;
  frames_ = 0;
  pushFrames_ = 0;
}

///-------------------------------------
/// Brief: Return if collider is dynamic
bool ColliderSquare::Dynamic()
{
  return isDynamic_;
}

///-------------------------------------
/// Brief: Return bounding box dimensions
Vec2D ColliderSquare::Dimensions()
{
  return dimensions_;
}

void ColliderSquare::Dimensions(Vec2D dim)
{
	dimensions_ = dim;
}

///-------------------------------------
/// Brief: Return block speed
float ColliderSquare::Speed()
{
  return speed_;
}

///-------------------------------------
/// Brief: Set collision status to true
void ColliderSquare::Collided()
{
  collided_ = true;
}

///-------------------------------------
/// Brief: Set collision status to false
void ColliderSquare::notCollided()
{
  collided_ = false;
}

///-------------------------------------
/// Brief: Resolution for square colliders
void ColliderSquare::SquareSquareResolution(int zone, bool is2Dynamic, std::string secondName)
{
  StaticResolution(secondName);
}