///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: CollisionManager.h
//
//  Author: Zack Keenly
//
//  Copyright © 2018 DigiPen (USA) Corporation
//
//  Brief:
//		The collision manager will check for collisions and call objects'
//		collision resolution functions.
//
//	Note:
//		In the future, the grid class will exist and be used for the semi-static
//		blocks in the push block puzzles.  In the event of a collision with a 
//		push block, the blocks position will need to be updated in the grid, the
//		player's velocity should match the blocks velocity, and the block should 
//		move, possibly an animation is necessary.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Obj.h"
#include "Vec2D.h"

///----------------------------------GLOBALS-----------------------------------
//typedef enum colliderType colliderType;

///---------------------------------FUNCTIONS-----------------------------------



class CollisionManager
{
public:

	//typedef enum Zone {
	//	Null_,
	//	TopRight_,
	//	Top_,
	//	TopLeft_,
	//	Left_,
	//	BottomLeft_,
	//	Bottom_,
	//	BottomRight_,
	//	Right_,
	//}Zone;

	CollisionManager();
	~CollisionManager();
	void Update();
	Vec2D GetPosition(Obj *object);
	bool CheckCollision(Obj *obj1, Obj *obj2);
	void CollisionHandler(Obj *obj1, Obj *obj2);
	int GetZone(Vec2D obj1Pos, Vec2D obj2pos);


private:
	std::vector<Obj *> Objects_;
	int ListSize_;
};
