///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: PhysicsSys.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "PhysicsSys.h"
#include "Spaces.h"
#include "ScreenManager.h"
#include "Engine.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

PhysicsSys::~PhysicsSys()
{
}

void PhysicsSys::Initialize()
{
}

void PhysicsSys::Update(float dt)
{
  //TraceMessage(false, "Physics system update");

  ScreenManager *screenManager = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));

  std::string name;
  //This is not the correct object vector,  the one in the object manager is the correct one; NEED SONYA
  std::vector<Obj*> ObjectList = screenManager->CurrentObjList();
  //PhysicsList &list = Space::GetPhysicsList();
  Physics *physics = nullptr;

  //Iterate through physics component map because only need to update physics components
  auto itEnd = ObjectList.end();

  for (auto it = ObjectList.begin(); it != itEnd; ++it)
  {
    physics = (*it)->Has(Physics);
    if (physics)
      physics->Update(dt);
  }
}