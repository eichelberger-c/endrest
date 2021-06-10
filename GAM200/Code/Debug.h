#pragma once
///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Debug.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"

#include <GLFW\glfw3.h>
#include "Obj.h"
#include "ColliderCircle.h"
#include "ColliderSquare.h"

///----------------------------------GLOBALS------------------------------------
typedef enum DebugMode
{
  OUTLINES,
  PHYSICS,
  BOTH,
  OFF,
  MESH,
}DebugMode;

///----------------------------------CLASSES------------------------------------
class Debug : public Component
{
public:
  Debug();
  Debug(const Debug& debug);
  Debug(Component *debug);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  ~Debug();
  void Initialize(Obj* parent);

  void updateMode();

private:

  DebugMode mode_ = OFF;

  bool init_ = true;

  Obj* outline_;

  Obj* vector_;
};

///---------------------------------FUNCTIONS-----------------------------------