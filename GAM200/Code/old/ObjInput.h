///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ObjInput.h
//
//  Author: Zack Keenly
//
//  Copyright © 2018 DigiPen (USA) Corporation
//
//  Functions:
//      - ObjInput
//      - ObjInput (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"
#include "Graphics.h"
#include "Vec2D.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class ObjInput : public Component
{
public:

  ObjInput();
  ObjInput(const Component& ogComponent);
  virtual Component * Clone() const;
  virtual void Update(float dt);
  void UpdateMouse(GLFWwindow *window);
  //Function to calculate object's new position based on CS230 work
  void CalcVelocity();

  friend class Physics;
  //friend class Collider;

private:
	//bool isTriggered_; //Probably don't actually need this
	Vec2D mousePos_; //Mouse position

};

///---------------------------------FUNCTIONS-----------------------------------


