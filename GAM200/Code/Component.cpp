///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Component.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Base component class for all components.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Component.h"
#include "Obj.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Constructor
Component::Component(compType type)
{
  type_ = type;
  isActive_ = true;
}

Component::Component(const Component& ogComponent) 
{

}

Component::Component(Component * comp)
{
}

void Component::Update(float dt)
{
  if (!isActive_)
    return;
}

///-------------------------------------
/// Brief: Set the parent Obj
void Component::Parent(Obj * parent)
{
  parent_ = parent;
}

///-------------------------------------
/// Brief: Get the component's parent Obj
Obj * Component::Parent()
{
  return parent_;
}

Obj * Component::Parent() const
{
  return parent_;
}

void Component::Type(compType type)
{
  type_ = type;
}

///-------------------------------------
/// Brief: Get the type
Component::compType Component::Type() const
{
  return type_;
}

bool Component::IsActive() const
{
  return isActive_;
}

void Component::IsActive(bool toggle)
{
  isActive_ = toggle;
}

///-------------------------------------
/// Brief: Compares two components. For the sort function
bool ComponentCompare(Component *left, Component *right)
{
  return (left->Type() < right->Type());
}
