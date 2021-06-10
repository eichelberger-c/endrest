///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ColliderSys.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This class handles checking for collisions with all objects.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "system.h"
#include "Vec2D.h"
#include "Collider.h"
#include "ColliderCircle.h"
#include "ColliderSquare.h"
///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class ColliderSys : public System
{
public:
  ColliderSys() : System("Collider") {};
  ~ColliderSys();
  virtual void Initialize();
  virtual void Update(float dt);

  Vec2D GetPosition(std::string &name);
  Vec2D GetPoint(std::string &firstName, std::string &secondName);
  void LightCollision(std::string player, std::string other);
  void CircleCircleCollision(ColliderCircle& collider1, ColliderCircle& collider2);
  void CircleSquareCollision(ColliderCircle& collider1, ColliderSquare& collider2);
  void SquareSquareCollision(ColliderSquare& collider1, ColliderSquare& collider2);
  bool PointCircle(Vec2D point, Collider *circle);
  bool LineCircle(Vec2D start, Vec2D end, Collider *circle, Transform *transform);
  bool PointLine(Vec2D start, Vec2D end, Vec2D point);
  void SetZone(std::string firstName, std::string secondName);

private:

};

///---------------------------------FUNCTIONS-----------------------------------


