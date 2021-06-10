///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: PhysicsSys.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "System.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class PhysicsSys : public System
{
public:
  PhysicsSys() : System("Physics") {};
  ~PhysicsSys();
  virtual void Initialize();
  virtual void Update(float dt);

private:

};

///---------------------------------FUNCTIONS-----------------------------------


