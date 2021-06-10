///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ObjFactory.cpp
//
//  Author: Sonya Toft
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
#include "ObjFactory.h"
#include "ObjManager.h"
#include "Engine.h"
#include "Serialization.h"


///----------------------------------GLOBALS-----------------------------------
using namespace std;

///---------------------------------FUNCTIONS-----------------------------------

void ObjFactory::Initialize()
{
  //AddCreator("Player", new PlayerCreator());
}

void ObjFactory::Update(float dt)
{
  ObjManager& manager = GetObjManager();
  manager.UpdateObjs(dt);
}

Obj * ObjFactory::CreateObject(std::string type)
{
  Obj *obj = BuildObject(type);
  obj->Initialize();

  ObjManagerAddObject(obj);

  return obj;
}

Obj * ObjFactory::CreateObjectFromFile(std::string filename)
{
  Obj *obj = BuildObjectFromFile(filename);

  ObjManagerAddObject(obj);

  return obj;
}

///-------------------------------------
/// Brief: Build an object from type (doesn't use serialization)
/// 
Obj * ObjFactory::BuildObject(std::string type)
{
  Obj *arch = Space::GetArchetype(type);

  return new Obj(*arch);
}

///-------------------------------------
/// Brief: Build an object from a file (uses serialization)
/// Only builds, doesn't initialize -> must be sent to Initializer
Obj * ObjFactory::BuildObjectFromFile(std::string filename)
{
  //stuff with serialization goes here
  
  return dynamic_cast<Data*>(EngineGetSystem("Data"))->FromJSON(filename);
}

///-------------------------------------
/// Brief: Adds a new creator type to the CreatorMap
//void ObjFactory::AddCreator(string type, ObjCreator * creator)
//{
//  pair<map<string, ObjCreator*>::iterator, bool> result;
//
//  result = creatorMap_.insert(pair<string, ObjCreator*>(type, creator));
//  if (result.second == false)
//    TraceMessage(false, "ObjFactory - Type duplicate %s attempted in CreatorMap.", type);
//
//  TraceMessage(false, "ObjFactory - Creator of type %s added to CreatorMap.", type);
//}

///-------------------------------------
/// Brief: Build an object from a file (uses serialization)
/// Builds an object from components specified in the file (custom object)
Obj * ObjFactory::CompositionFactory::BuildObject(std::string filename)
{
  //more serialization stuff goes here too

  return nullptr;
}

///-------------------------------------
/// Brief: Adds a component creator type to CompMap
void ObjFactory::CompositionFactory::AddCompCreator(std::string type, CompCreator * creator)
{
  pair<map<string, CompCreator*>::iterator, bool> result;

  result = compMap_.insert(pair<string, CompCreator*>(type, creator));
  if (result.second == false)
    TraceMessage(TraceLog, "CompFactory - Component type duplicate %s attempted in CreatorMap.", type);

  TraceMessage(TraceLog, "CompFactory - Component creator of type %s added to CompMap.");
}
