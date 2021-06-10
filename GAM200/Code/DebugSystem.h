///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: DebugSystem.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This class handles the debug components 
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "System.h"
#include "Obj.h"
#include "Spaces.h"
#include "Debug.h"
///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class DebugSystem : public System
{
public:
  DebugSystem() : System("DebugSystem") {};
  ~DebugSystem() {};
  virtual void Initialize();
  virtual void Update(float dt);

  inline bool ChangeMode() { return isDirty_; }
  inline void ChangeMode(bool isDirty) { isDirty_ = isDirty; }
private:
  bool isDirty_ = false;
};