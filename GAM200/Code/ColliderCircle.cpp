///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ColliderCircle.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -ColliderCircle(float radius)
//		-ColliderCircle(const Component& ogComponent)
//		-virtual Component * Clone() const
//		-virtual void Update(float dt)
//		-void CircleCircleResolution(std::string name)
//		-void CircleSquareResolution(int zone, bool isDynamic, std::string firstName, std::string secondName)
//		-float Radius()
//		-bool Dynamic()
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#define _USE_MATH_DEFINES
#include <cmath>
#include "stdafx.h"
#include "ColliderCircle.h"
#include "Spaces.h"
#include "Graphics.h"
#include "Area.h"
//#include "EventComp.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Initialize collider
ColliderCircle::ColliderCircle(float radius, bool dynamic, bool block)
{
  frames_ = 0;
  collided_ = false;
  radius_ = radius;
  isDynamic_ = dynamic;
  isBlock_ = block;
  flash_ = 120;
  timer_ = 30;
  pushFrames_ = 0;
}

///-------------------------------------
/// Brief: Copy Constructor
ColliderCircle::ColliderCircle(const ColliderCircle & ogComponent)
{
  frames_ = ogComponent.frames_;
  collided_ = false;
  radius_ = ogComponent.radius_;
  isDynamic_ = ogComponent.isDynamic_;
  flash_ = ogComponent.flash_;
  timer_ = ogComponent.timer_;
  pushFrames_ = ogComponent.pushFrames_;
  isBlock_ = ogComponent.isBlock_;
}

///-------------------------------------
/// Brief: Copy Constructor
ColliderCircle::ColliderCircle(ColliderCircle * circle)
{
  ColliderCircle *newCircle = reinterpret_cast<ColliderCircle*>(circle);
  frames_ = newCircle->frames_;
  collided_ = false;
  radius_ = newCircle->radius_;
  isDynamic_ = newCircle->isDynamic_;
  flash_ = newCircle->flash_;
  timer_ = newCircle->timer_;
  pushFrames_ = newCircle->pushFrames_;
  isBlock_ = newCircle->isBlock_;
}

///-------------------------------------
/// Brief: Clone Collider
Component * ColliderCircle::Clone() const
{
  ColliderCircle *circle = new ColliderCircle(*this);
  if (circle == nullptr)
    return nullptr;

  //*circle = *this;

  return circle;
}

///-------------------------------------
/// Brief: Detects if collision resolution is still
///			occurring and updates it
void ColliderCircle::Update(float dt)
{
  if (!IsActive())
    return;

  Obj * parent = Parent();
  std::string name = parent->Name();
  Physics *physics = Space::GetPhysics(name);
  if(physics)
    Vec2D Velocity = physics->Velocity();

  if (collided_)
  {
    frames_--;
    //flash_--;
    //timer_--;

    if (isBlock_)
    {
      pushFrames_--;

      if (pushFrames_ <= 0)
      {
        if(physics)
          physics->Velocity(0.0f, 0.0f);
        collided_ = false;
        pushFrames_ = 0;
      }
    }

    if (frames_ <= 0)
    {
      if (physics)
        physics->Velocity(0.0f, 0.0f);
      collided_ = false;
      frames_ = 0;
      //flash_ = 120;
      //timer_ = 30;
    }
  }


}

///-------------------------------------
/// Brief: Resets properties of collider to
///        when first constructed.
void ColliderCircle::Reset()
{
  frames_ = 0;
  collided_ = false;
  flash_ = 120;
  timer_ = 30;
  pushFrames_ = 0;
}

///-------------------------------------
/// Brief: Returns collider radius
float ColliderCircle::Radius()
{
  return radius_;
}

///-------------------------------------
/// Brief: Returns whether collider is dynamic.
///			In the future we may want circles to 
///			be static, for behavior or effect
bool ColliderCircle::Dynamic()
{
  return isDynamic_;
}

///-------------------------------------
/// Brief: Returns whether object has collided
bool ColliderCircle::Collided()
{
  return collided_;
}

void ColliderCircle::Collided(bool argument)
{
  collided_ = argument;
}

void ColliderCircle::notCollided()
{
  collided_ = false;
}

bool ColliderCircle::isBlock()
{
  return isBlock_;
}

int ColliderCircle::Frames()
{
  return frames_;
}

void ColliderCircle::SetFrames(int frames)
{
  frames_ = frames;
}

void ColliderCircle::SetPushFrames(int frames)
{
  pushFrames_ = frames;
}

///-------------------------------------
/// Brief: Collision resolution for circle and circle collision
void ColliderCircle::CircleCircleResolution(Obj* object2)
{
  //Check for self
  if (object2->Name() == Parent()->Name())
    return;

  if (!IsActive() || !object2->IsActive())
    return;

  ColliderCircle* circle2 = static_cast<ColliderCircle*>(object2->Has(Collider));

  if (collided_ || circle2->collided_)
    return;

  Physics *myPhysics = Parent()->Has(Physics);
  Physics *otherPhysics = object2->Has(Physics);
  Transform* myTransform = Parent()->Has(Transform);
  Transform *otherTransform = object2->Has(Transform);

  Vec2D myLoc = myTransform->Translation();
  Vec2D otherLoc = otherTransform->Translation();

  Vec2D angleVec = myLoc - otherLoc;

  if (circle2->isBlock_)
  {
    Vec2D vel = myPhysics->Velocity();
    vel *= -.1f;
    myPhysics->Velocity(vel);
    collided_ = true;
    frames_ = 10;
  }

  if (circle2->isDynamic_)
  {
    if (glfwGetKey(Graphics::Window(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      double angle = glm::atan(angleVec.Y(), angleVec.X());
      angle *= 180;
      angle = angle / M_PI;
      if (angle > -45 && angle < 45)
        otherPhysics->Velocity(-100, 0);
      else if (angle > 45 && angle < 135)
        otherPhysics->Velocity(0, -100);
      else if (angle > -135 && angle < -45)
        otherPhysics->Velocity(0, 100);
      else
        otherPhysics->Velocity(100, 0);
      circle2->collided_ = true;
      circle2->frames_ = 10;
      circle2->pushFrames_ = 15;
    }
  }

  if ((object2->Name().find("Win") != std::string::npos && Parent()->Name().find("Push") != std::string::npos))
  {
    myTransform->Translation(otherTransform->Translation());
    isDynamic_ = false;
  }
  else if (object2->Name().find("Push") != std::string::npos && Parent()->Name().find("Win") != std::string::npos)
  {
    otherTransform->Translation(myTransform->Translation());
    circle2->isDynamic_ = false;
  }
}

void ColliderCircle::ResolveWall(ColliderSquare* square)
{
	Vec2D otherPos = square->Parent()->Has(Transform)->Translation();
	Vec2D objLoc = Parent()->Has(Transform)->Translation();
	Vec2D push = objLoc - otherPos;
	collided_ = true;
	Physics* phys = Parent()->Has(Physics);
	phys->Velocity(push);
	frames_ = 3;
	pushFrames_ = 10;
}

void ColliderCircle::Push(ColliderCircle* other)
{
	Vec2D otherPos = other->Parent()->Has(Transform)->Translation();
	Vec2D objLoc = Parent()->Has(Transform)->Translation();
	Vec2D push = objLoc - otherPos;
	collided_ = true;
	Physics* phys = Parent()->Has(Physics);
	phys->Velocity(push);
	frames_ = 10;
	pushFrames_ = 10;
}

void ColliderCircle::LanternPush()
{
	Lantern* lant = static_cast<Lantern*>((&Space::GetPlayer())->GetSubObject("Lantern"));

	Transform* lantTrans = lant->Has(Transform);
	Vec2D lantPos = lantTrans->Translation();
	Vec2D pos = Parent()->Has(Transform)->Translation();

	Vec2D push = pos - lantPos;
	Parent()->Has(Physics)->Velocity(push);
	collided_ = true;
	frames_ = 10;
}

























//Deprecated but perhaps still useful code
////This is code for the flashing resolution requested by Cam; may use again
////A brief moment where player stays solid
//if (timer_ <= 0)
//{
//  //Oscillate between two different states to indicate invulnerability
//  if (parent->Name() == "Player")
//  {
//    if (flash_ % 8 == 0)
//    {
//      objSprite->SetAlpha(0.75); //Less visible
//    }
//    else
//    {
//      objSprite->SetAlpha(1.0); //Visible
//    }
//  }
//}