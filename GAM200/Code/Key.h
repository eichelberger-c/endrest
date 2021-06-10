///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Key.h
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: stores information needed by objects with key behaviors
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class Key : public Component
{
public:
  Key(std::string id = "TEST");
  Key(const Key& key);
  virtual Component * Clone() const;
  virtual void Update(float dt) {};

  void ID(std::string id);
  std::string ID();

private:
  std::string id_;
};

///---------------------------------FUNCTIONS-----------------------------------


