///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Collider.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This file contains the interface for the Collider class.  Originally
//			There were supposed to be separate collider classes, however that
//			will have to be implemented after milestone 1.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
//#define Has(type) GetComp<type>(type)
#include "Component.h"

///----------------------------------GLOBALS------------------------------------
//typedef class GameObjectManager GameObjectManager;

///----------------------------------CLASSES------------------------------------

class Collider : public Component
{
public:

  Collider();
  ~Collider();
  Collider(const Collider &ogcollider) : Component(Collider_) {};
  Collider(Component *collider) : Component(Collider_) {};
  void StaticResolution(std::string &secondObj);
  virtual void Update(float dt);
  virtual void Reset() {};

private:
  
};