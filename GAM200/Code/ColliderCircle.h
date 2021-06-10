///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ColliderCircle.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  brief: Circle Colliders are for dynamic objects, the player, enemies, and possibly
//		   other types of objects will have circle colliders.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Collider.h"
#include "Vec2D.h"
#include "ScreenManager.h"
#include "Engine.h"
#include "ColliderSquare.h"
///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class ColliderCircle : public Collider
{
public:
  ColliderCircle(float radius, bool dynamic, bool block = false);
  ColliderCircle(const ColliderCircle& ogComponent);
  ColliderCircle(ColliderCircle* circle);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset();
  void CircleCircleResolution(Obj* obj);
  float Radius();
  bool Dynamic();
  bool Collided();
  void Collided(bool argument);
  void notCollided();
  bool isBlock();
  int Frames();
  void SetFrames(int frames);
  void SetPushFrames(int frames);
  void ResolveWall(ColliderSquare*);
  void Push(ColliderCircle*);
  void LanternPush();
  //friend class Input;
  friend class ColliderSys;

private:
  int frames_;
  int pushFrames_;
  int timer_;
  unsigned int flash_;
  float radius_;
  bool collided_; //Use this to keep track of frames for circle-circle collision resolution
  bool isBlock_;
  bool isDynamic_;
  ScreenManager *scrnManager_ = reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
 
};

///---------------------------------FUNCTIONS-----------------------------------


