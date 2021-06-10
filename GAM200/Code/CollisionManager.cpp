///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: CollisionManager.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2018 DigiPen (USA) Corporation
//
//  Functions:
//		-CollisionManager();
//		-~CollisionManager();
//		-void Update();
//		-Vec2D GetPosition(Obj *object);
//		-bool CheckCollision(Obj *obj1, Obj *obj2);
//		-void CollisionHandler(Obj *obj1, Obj *obj2);
//		-int GetZone(Vec2D obj1Pos, Vec2D obj2pos);
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "CollisionManager.h"
//#include "Grid.h"
#include "Collider.h"
//#include "ColliderCircle.h"
//#include "ColliderSquare.h"
//#include "Vec2D.h"
#include "Transform.h"
#include "Trace.h"


///----------------------------------GLOBALS-----------------------------------
//typedef enum colliderType colliderType;

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Constructor
CollisionManager::CollisionManager()
{
}

///-------------------------------------
/// Brief: Deconstructor
CollisionManager::~CollisionManager()
{
}

///-------------------------------------
/// Brief: Update function runs through object array and checks for collisions
void CollisionManager::Update()
{
	for (int i = 0; i < ListSize_; ++i)
	{
		for (int j = i + 1; j < ListSize_; ++j)
		{
			if (CheckCollision(Objects_[i], Objects_[j]))
				CollisionHandler(Objects_[i], Objects_[j]);
		}
	}
}

///-------------------------------------
/// Brief: Get's position of given object
Vec2D CollisionManager::GetPosition(Obj *object)
{
	//get object's transform to call Position()
	Transform *transform = reinterpret_cast<Transform*>(object->Get(Component::Transform_));
	
	//Create a Vec2D
	Vec2D position = transform->Position();
	
	//return new Vec2D
	return position;
}

///-------------------------------------
/// Brief: Checks if collision occurred
bool CollisionManager::CheckCollision(Obj *obj1, Obj *obj2)
{
	//Get positions of objects
	Vec2D obj1Pos = GetPosition(obj1);
	Vec2D obj2Pos = GetPosition(obj2);

	//Check obj2's zone
	int obj1Zone = GetZone(obj1Pos, obj2Pos);
	int obj2Zone = GetZone(obj2Pos, obj1Pos);

	//Get objects' colliders
	Collider *collider1 = reinterpret_cast<Collider*>(obj1->Get(Component::Collider_));
	Collider *collider2 = reinterpret_cast<Collider*>(obj2->Get(Component::Collider_));

	//Determine Collider types
	colliderType_ obj1Type = collider1->ReturnColliderType();
	colliderType_ obj2Type = collider2->ReturnColliderType();

	float distance = obj1Pos ^ obj2Pos;
	

	//Check for a collision
	switch (obj1Type)
	{
	case cCircle_:
		if (obj2Type == cCircle_)
		{
			/*
			If the sum of the radii is greater than the distance then they must be colliding
			*/
			circleC *obj1Circle = collider1->GetCircleCollider();
			circleC *obj2Circle = collider2->GetCircleCollider();
			float sum = obj1Circle->radius_ + obj2Circle->radius_;
			
			if (sum > distance)
				return true;

			break;
		}

		if (obj2Type == cSquare_)
		{
			//Get Colliders
			circleC *obj1Circle = collider1->GetCircleCollider();
			squareC *obj2Square = collider2->GetSquareCollider();

			//Get Radius
			float radius = obj1Circle->radius_;

			//Create a vector for the edge of the circle
			Vec2D corner;

			switch (obj1Zone)
			{
			case TopRight_:
				//Vec2D corner;
				corner.Set(obj1Pos.x_ + radius, obj1Pos.y_ + radius);
				if (corner.x_ > (obj2Pos.x_ - obj2Square->dimensionX_))
					if (corner.y_ > (obj2Pos.y_ - obj2Square->dimensionY_))
					{
						TraceMessage(true, "Collision Hath Occurred");
						return true;
					}
				break;
			case Top_:
				//Vec2D corner;
				corner.Set(obj1Pos.x_, obj1Pos.y_ + radius);
				if (corner.y_ > (obj2Pos.y_ - obj2Square->dimensionY_))
				{
					TraceMessage(true, "Collision Hath Occurred");
					return true;
				}
				break;
			case TopLeft_:
				//Vec2D corner;
				corner.Set(obj1Pos.x_ - radius, obj1Pos.y_ + radius);
				if (corner.x_ < (obj2Pos.x_ + obj2Square->dimensionX_))
					if (corner.y_ > (obj2Pos.y_ - obj2Square->dimensionY_))
					{
						TraceMessage(true, "Collision Hath Occurred");
						return true;
					}
				break;
			case Left_:
				//Vec2D corner;
				corner.Set(obj1Pos.x_ - radius, obj1Pos.y_);
				if (corner.x_ < (obj2Pos.x_ + obj2Square->dimensionX_))
				{
					TraceMessage(true, "Collision Hath Occurred");
					return true;
				}
				break;
			case BottomLeft_:
				//Vec2D corner;
				corner.Set(obj1Pos.x_ - radius, obj1Pos.y_ - radius);
				if (corner.x_ < (obj2Pos.x_ + obj2Square->dimensionX_))
					if (corner.y_ < (obj2Pos.y_ + obj2Square->dimensionY_))
					{
						TraceMessage(true, "Collision Hath Occurred");
						return true;
					}
				break;
			case Bottom_:
				//Vec2D corner;
				corner.Set(obj1Pos.x_, obj1Pos.y_ - radius);
				if (corner.y_ < (obj2Pos.y_ + obj2Square->dimensionY_))
				{
					TraceMessage(true, "Collision Hath Occurred");
					return true;
				}
				break;
			case BottomRight_:
				//Vec2D corner;
				corner.Set(obj1Pos.x_ + radius, obj1Pos.y_ - radius);
				if (corner.x_ > (obj2Pos.x_ - obj2Square->dimensionX_))
					if (corner.y_ < (obj2Pos.y_ + obj2Square->dimensionY_))
					{
						TraceMessage(true, "Collision Hath Occurred");
						return true;
					}
				break;
			case Right_:
				//Vec2D corner;
				corner.Set(obj1Pos.x_ + radius, obj1Pos.y_);
				if (corner.x_ > (obj2Pos.x_ - obj2Square->dimensionX_))
				{
					TraceMessage(true, "Collision Hath Occurred");
					return true;
				}
				break;
			}
			break;
		}

		break;
	case cSquare_:
		//As of right now, nothing
		break;
	}

	return false;
}

///-------------------------------------
/// Brief: Calls collision resolution functions for both objects
void CollisionManager::CollisionHandler(Obj *obj1, Obj *obj2)
{
	//Get positions of objects
	Vec2D obj1Pos = GetPosition(obj1);
	Vec2D obj2Pos = GetPosition(obj2);

	//Check obj2's zone
	int obj1Zone = GetZone(obj1Pos, obj2Pos);
	int obj2Zone = GetZone(obj2Pos, obj1Pos);

	//Get objects' colliders
	Collider *collider1 = reinterpret_cast<Collider*>(obj1->Get(Component::Collider_));
	Collider *collider2 = reinterpret_cast<Collider*>(obj2->Get(Component::Collider_));

	//Determine Collider types
	colliderType_ obj1Type = collider1->ReturnColliderType();
	colliderType_ obj2Type = collider2->ReturnColliderType();

	collider1->CollisionResolution(obj1Type, obj1Zone, collider1->GetDynamic(obj1Type), obj2);
	collider2->CollisionResolution(obj2Type, obj2Zone, collider2->GetDynamic(obj2Type), obj1);

	/*switch (obj1Type)
	{
	case cCircle_:
		switch (obj2Type)
		{
		case cCircle_:
			collider1->CollisionResolution(obj1Type, obj1Zone, collider1->GetDynamic(obj1Type));
			collider2->CollisionResolution(obj2Type, obj2Zone, collider2->GetDynamic(obj2Type));
			break;
		case cSquare_:
			collider1->CollisionResolution(obj1Type, obj1Zone, collider1->GetDynamic(obj1Type));
			collider2->CollisionResolution(obj2Type, obj2Zone, collider2->GetDynamic(obj2Type));
			break;
		}
		break;
	case cSquare_:
		break;
	}*/
}

///-------------------------------------
/// Brief: Gets relative zones for each object
int CollisionManager::GetZone(Vec2D obj1Pos, Vec2D obj2Pos)
{
	//Get object's position and determine where it is in reference to this object
	//By getting the values of their positions and checking for positions within 
	//a range because it will never be exactly above it besides one instant
	float X1 = obj1Pos.x_;
	float Y1 = obj1Pos.y_;
	float X2 = obj2Pos.x_;
	float Y2 = obj2Pos.y_;
	int Zone = 0;
	float range = 1.5f;

	/*Top right*/
	if (X2 > X1 && Y2 > X1)
	{
		Zone = TopRight_;
		return Zone;
	}
	/*Top*/
	if (X2 < (X1 + range) && X2 > (X1 - range) && Y2 > Y1)
	{
		Zone = Top_;
		return Zone;
	}
	/*Top left*/
	if (X2 < X1 && Y2 > Y1)
	{
		Zone = TopLeft_;
		return Zone;
	}
	/*Left*/
	if (X2 < X1 && Y2 < (Y1 + range) && Y2 > (Y1 - range))
	{
		Zone = Left_;
		return Zone;
	}
	/*Bottom left*/
	if (X2 < X1 && Y2 < Y1)
	{
		Zone = BottomLeft_;
		return Zone;
	}
	/*Bottom*/
	if (X2 < (X1 + range) && X2 >(X1 - range) && Y2 < Y1)
	{
		Zone = Bottom_;
		return Zone;
	}
	/*Bottom right*/
	if (X2 > X1 && Y2 < Y1)
	{
		Zone = BottomRight_;
		return Zone;
	}
	/*Right*/
	if (X2 > X1 && Y2 < (Y1 + range) && Y2 >(Y1 - range))
	{
		Zone = Right_;
		return Zone;
	}

	return cNull_;
}