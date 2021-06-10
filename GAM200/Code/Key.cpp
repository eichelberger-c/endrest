///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Key.cpp
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Key
//      - ~Key
//      - Clone
//      - Update
//      - DoorArea
//      - DoorID
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Engine.h"
#include "ScreenManager.h"
#include "Key.h"

///----------------------------------GLOBALS-----------------------------------

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: key constructor
Key::Key(std::string id) : Component(Key_), id_(id)
{
}

///------------------------------------- 
/// Brief: key copy constructor
Key::Key(const Key & key) : Component(Key_), id_(key.id_)
{
}

///-------------------------------------
/// Brief: clones key
Component * Key::Clone() const
{
  return nullptr;
}

///-------------------------------------
/// Brief: sets key id
void Key::ID(std::string id)
{
  id_ = id;
}

///-------------------------------------
/// Brief: gets key id
std::string Key::ID()
{
  return id_;
}
