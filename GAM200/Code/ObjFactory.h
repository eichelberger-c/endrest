///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ObjFactory.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: In charge of creating objects.
//  Can be done from type, a file, or a component list
//  Has a map of specialized creator classes that it passes obj requests to
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include "CompCreator.h"
#include <string>
#include "Spaces.h"
#include "System.h"
#include "ObjManager.h"

///----------------------------------GLOBALS------------------------------------
struct Comp
{
  bool operator() (const std::string& lhs, const std::string& rhs) const
  {
    return lhs < rhs;
  }
};

///----------------------------------CLASSES------------------------------------
class ObjFactory : public System
{
public:
  ObjFactory() : System("ObjFactory") {}
  virtual void Initialize();
  virtual void Update(float dt);

  Obj * CreateObject(std::string type);
  Obj * CreateObjectFromFile(std::string filename);
  Obj * BuildObject(std::string type);
  Obj * BuildObjectFromFile(std::string filename);
  //void AddCreator(std::string type, ObjCreator *creator);

  ///this doesn't do anything right now so leave it alone
  class CompositionFactory
  {
  public:
    Obj * BuildObject(std::string filename);
    void AddCompCreator(std::string type, CompCreator *creator);

  private:
    std::map<std::string, CompCreator*, Comp> compMap_;
  };

private:
  //std::map<std::string, ObjCreator*, Comp> creatorMap_;
};


///---------------------------------FUNCTIONS-----------------------------------


