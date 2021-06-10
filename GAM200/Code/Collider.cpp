///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Collider.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//    - Collider();
//		- Collider(colliderType type, float radius, float dimensionX, float dimensionY, bool isDynamic);
//		- ~Collider();
//		- virtual void Update(float dt);
//		- colliderType_ ReturnColliderType();
//		- bool GetDynamic(colliderType type);
//		- void SetAcceleration();
//		- circleC* GetCircleCollider();
//		- squareC* GetSquareCollider();
//		- void CollisionResolution(colliderType type, int zone, bool isDynamic, Obj *obj2);
//		- void CircleCircleResolution();
//		- void CircleSquareResolution(int zone, bool isDynamic, Obj *obj2);
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Collider.h"
#include "Transform.h"
#include "Physics.h"
#include "Spaces.h"

///----------------------------------GLOBALS-----------------------------------



///---------------------------------FUNCTIONS-----------------------------------


///-------------------------------------
/// Brief: Constructor
Collider::Collider() : Component(Collider_)
{
  //This constructor should NEVER be called!!!
  //When creating a collider it NEEDS to be specified!!!
}

///-------------------------------------
/// Brief: Destructor
Collider::~Collider()
{
}

void Collider::StaticResolution(std::string & secondObj)
{
  //translations
  Transform *myTransform = Space::GetTransform(Parent()->Name());
  Transform *objTransform = Space::GetTransform(secondObj);

  //physics
  Physics *myPhysics = Space::GetPhysics(Parent()->Name());
  Vec2D vel = myPhysics->Velocity();
  vel *= .1f;
  myPhysics->Velocity(vel);

  //Vec2D newVelocity(0,0);
  //Vec2D newTranslation;
  //Vec2D myTranslation = myTransform->Translation();
  //Vec2D objTranslation = objTransform->Translation();

  //myPhysics->Velocity(newVelocity);

  //newTranslation = myTranslation - objTranslation;
  //newTranslation *= 1.00015f;
  ////PlayerInputIsActive(false);

  //myTransform->Translation(objTranslation + newTranslation);
}

///-------------------------------------
/// Brief: Virtual function; can't think of a use for it
void Collider::Update(float dt)
{
  if (!IsActive())
    return;
}