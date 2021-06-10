///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Physics.cpp
//
//  Author: Sonya Toft
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Brief:
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Physics.h"
#include "Spaces.h"
#include "Transform.h"
#include "Obj.h"
#include <glm\gtc\matrix_transform.hpp>
#include "Trace.h"
#include "Graphics.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

Physics::Physics() : Component(Component::Physics_)
{
  oldPosition_.x_ = 0.0f;
  oldPosition_.y_ = 0.0f;
  velocity_.x_ = 0.0f;
  velocity_.y_ = 0.0f;
  acceleration_ = 0.0f;

  /*constructor_ = &ConstructPhysics;*/

  //Space::AddPhysics(this);
}

Physics::Physics(Vec2D velocity, float acceleration) : Component(Physics_)
{
  oldPosition_.x_ = 0.0f;
  oldPosition_.y_ = 0.0f;
  velocity_ = velocity;
  acceleration_ = acceleration;

  /*constructor_ = &ConstructPhysics;*/
}

Physics::Physics(float velX, float velY, float accel) : Component(Physics_)
{
  oldPosition_ = { 0.0f };
  velocity_.x_ = velX;
  velocity_.y_ = velY;
  acceleration_ = accel;

  /*constructor_ = &ConstructPhysics;*/
}

Physics::Physics(const Physics& ogPhysics) : Component(Component::Physics_)
{
  oldPosition_ = ogPhysics.oldPosition_;
  velocity_ = ogPhysics.velocity_;
  acceleration_ = ogPhysics.acceleration_;
  IsActive(ogPhysics.IsActive());

  //Space::AddPhysics(this);
}

Physics::Physics(Component * phys) : Component(Component::Physics_)
{
  Physics *physics = dynamic_cast<Physics*>(phys);

  oldPosition_ = physics->oldPosition_;
  velocity_ = physics->velocity_;
  acceleration_ = physics->acceleration_;
  IsActive(phys->IsActive());
}

Component * Physics::Clone() const
{
  Physics *phys = new Physics;
  if (phys == NULL)
    return NULL;

  *phys = *this;

  return phys;
}

///------------------------------------------
///Author: Zack Keenly
///Brief: Updates the physics component
void Physics::Update(float dt)
{
  if (!IsActive())
    return;

  //Get translation component
  Transform *transform = Parent()->Has(Transform);

  //Get position
  oldPosition_ = transform->translation_;
  Vec2D newTranslation(oldPosition_);

  //Calculate new position
  newTranslation.x_ += (velocity_.x_ * dt);
  newTranslation.y_ += (velocity_.y_ * dt);

  //To update transform component with new velocity
  transform->isDirty_ = true;
  transform->Translation(newTranslation);

  //transform->Update(dt);
  //TransformSetTranslation(transform, &newTranslation);
}

void Physics::Reset()
{
  Velocity(0.0f, 0.0f);
}

///-------------------------------------
/// Brief: Get old position
Vec2D Physics::OldPosition()
{
  return oldPosition_;
}

///-------------------------------------
/// Brief: Get velocity
Vec2D Physics::Velocity()
{
  return velocity_;
}

glm::vec2 Physics::GlmVelocity()
{
  return glm::vec2(velocity_.X(), velocity_.Y());
}

///-------------------------------------
/// Brief: Get acceleration
float Physics::Acceleration()
{
  return acceleration_;
}

void Physics::Position(Vec2D position)
{
  oldPosition_ = position;
}

///-------------------------------------
/// Brief: Set Velocity
void Physics::Velocity(Vec2D velocity)
{
  velocity_.Set(velocity.x_, velocity.y_);
}

void Physics::Velocity(glm::vec2 velocity)
{
  velocity_.Set(velocity.x, velocity.y);
}

void Physics::Velocity(float X, float Y)
{
  velocity_.Set(X, Y);
}

void Physics::MultAcceleration(float multiplier)
{
  acceleration_ *= multiplier;
}

void Physics::Acceleration(float acceleration)
{
  acceleration_ = acceleration;
}

//Component * Physics::Construct()
//{
//  return new Physics;
//}

Component * ConstructPhysics(void)
{
  return new Physics;
}
