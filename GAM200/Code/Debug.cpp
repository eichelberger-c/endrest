///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Debug.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#define _USE_MATH_DEFINES
#include <cmath>
#include "stdafx.h"
#include "Transform.h"
#include "Sprite.h"
#include "Physics.h"
#include "Debug.h"
#include "Lantern.h"
#include "Spaces.h"

///----------------------------------GLOBALS-----------------------------------
static bool f = false;
static bool t = true;


///---------------------------------FUNCTIONS-----------------------------------

Debug::Debug() : Component(Debug_)
{
}

Debug::Debug(const Debug & debug) : Component(Debug_)
{
  init_ = true;
}

Debug::Debug(Component * debug) : Component(Debug_)
{
  init_ = true;
}

Component * Debug::Clone() const
{
  return new Debug(*this);
}

void Debug::Initialize(Obj* parent)
{
  Collider* collider = parent->Has(Collider);
  Physics* phys = parent->Has(Physics);

  if (collider)
  {
    ColliderCircle* circle = dynamic_cast<ColliderCircle*>(collider);
    ColliderSquare* square = dynamic_cast<ColliderSquare*>(collider);
    outline_ = new Obj("DebugOutline");
    if (circle)
      outline_->Add(new Sprite("Data/Textures/Collider.png"));

    if (square)
      outline_->Add(new Sprite("Data/Textures/ColliderSquare.png"));

    outline_->Add(new Transform(0,0));
    outline_->IsActive(f);
    parent->AddSubObj(outline_);
  }

  if (phys)
  {
    vector_ = new Obj("DebugSpeedVector");
    vector_->Add(new Sprite("Data/Textures/Vector.png"));
    vector_->Add(new Transform(0, 0));
    vector_->IsActive(f);
    parent->AddSubObj(vector_);
  }
}

Debug::~Debug()
{
  //if(vector_)
  //  delete vector_;
  //if(outline_)
  //  delete outline_;
}

void Debug::Update(float dt)
{
  if (init_)
  {
    Initialize(Parent());
    init_ = false;
  }

  if (mode_ == OFF)
    return;

  Obj* parent = Parent();
  Transform* parentTrans = parent->Has(Transform);

  if (parentTrans)
  {
    if (vector_ && (mode_ == PHYSICS || mode_ == BOTH))
    {
      Physics* parentPhys = parent->Has(Physics);
      Transform* vecTrans = vector_->Has(Transform);
      glm::vec2 vel = parentPhys->GlmVelocity();
      if (vel.x == 0 && vel.y == 0)
        vector_->Has(Sprite)->SetVisible(f);
      else
        vector_->Has(Sprite)->SetVisible(t);
      float angle = glm::atan(vel.y, vel.x);
      vecTrans->Rotation(angle);
      vector_->Update(dt);
      vecTrans->Translation(parentTrans->Translation());
    }

    if (outline_ && (mode_ == OUTLINES || mode_ == BOTH))
    {
      Collider* collider = parent->Has(Collider);
      ColliderCircle* circle = dynamic_cast<ColliderCircle*>(collider);
      ColliderSquare* square = dynamic_cast<ColliderSquare*>(collider);
      Transform* outTrans = outline_->Has(Transform);
      Vec2D scale;

      if (circle)
      {
        scale.X(2 * circle->Radius());
        scale.Y(2 * circle->Radius());
      }

      if (square)
      {
        scale = square->Dimensions();
      }

      outline_->Update(dt);
      outTrans->Translation(parentTrans->Translation());
      outTrans->Scale(scale.X(), scale.Y());
    }
  }
}

void Debug::updateMode()
{
  switch (mode_)
  {
    case OFF:
    {
      mode_ = OUTLINES;
      if(outline_)
        outline_->IsActive(t);
      break;
    }
    case OUTLINES:
    {
      mode_ = PHYSICS;
      if (outline_)
        outline_->IsActive(f);
      if(vector_)
        vector_->IsActive(t);
      break;
    }
    case PHYSICS:
    {
      mode_ = BOTH;
      if (outline_)
        outline_->IsActive(t);
      break;
    }
    case BOTH:
    {
      mode_ = OFF;
      if (outline_)
        outline_->IsActive(f);
      if (vector_)
        vector_->IsActive(f);
      break;
    }
  }
}