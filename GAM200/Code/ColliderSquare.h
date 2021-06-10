///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ColliderSquare.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  brief: This is an abstracted class for the collision functionality.  Square
//		   colliders are for static objects and push blocks.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Collider.h"
#include "Vec2D.h"

///----------------------------------GLOBALS------------------------------------
typedef enum orientation {
  none_,
  top_,
  left_,
  bottom_,
  right_
}Orientation;

///----------------------------------CLASSES------------------------------------

class ColliderSquare : public Collider
{
public:
  ColliderSquare(float dimensionX, float dimensionY, bool dynamic, bool block);
  ColliderSquare(const ColliderSquare& ogComponent);
  ColliderSquare(ColliderSquare *square);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset();
  void SquareSquareResolution(int zone, bool isDynamic, std::string secondName);
  bool Dynamic();
  Vec2D Dimensions();
  void Dimensions(Vec2D dim);
  float Speed();
  void Collided();
  void notCollided();

  //friend class Input;
  friend class ColliderSys;

private:
  Vec2D dimensions_;
  bool isDynamic_;
  bool collided_;
  bool isBlock_;
  float speed_;
  int frames_;
  int pushFrames_;
  //ScreenManager *scrnManager_ = reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
};

///---------------------------------FUNCTIONS-----------------------------------


