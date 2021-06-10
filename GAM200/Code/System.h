///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: System.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Virtual System class for all systems to inherit from
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class System
{
public:
  System(const char *name); //{ name_ = name; }
  virtual ~System() {}
  virtual void Initialize() {}
  virtual void Update(float dt) {}
  std::string Name() { return name_; };
  //send message function(?)

private:
  std::string name_;
};

///---------------------------------FUNCTIONS-----------------------------------
