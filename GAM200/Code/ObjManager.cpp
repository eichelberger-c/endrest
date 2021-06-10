///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ObjManager.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//    - ObjManager();
//		- ~ObjManager();
//		- void AddObject(Obj *gameObject);
//		- void UpdateObjects();
//		- Obj* GetObject(const char *name);
//		- bool CheckName(const char *name);
//		- Obj* GetNextObject(int index);
//		- int GetListSize();
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "ObjManager.h"
#include "Spaces.h"
#include "Engine.h"
#include "ScreenManager.h"

///----------------------------------GLOBALS-----------------------------------
ObjManager objectManager;

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Constructor
ObjManager::ObjManager()
{
  place_ = 0;
}

///-------------------------------------
/// Brief: Destructor
ObjManager::~ObjManager()
{
  int size = Space::ObjList.size();
  for (int i = 0; i < size; ++i)
    delete Space::ObjList[i];
}

///-------------------------------------
/// Brief: Adds objects to object array
void ObjManager::AddObj(Obj * gameObject)
{
  Space::ObjList.push_back(gameObject);
}

///-------------------------------------
/// Brief: Walks through array and updates objects
void ObjManager::UpdateObjs(float dt)
{
  ObjectList objs = reinterpret_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->CurrentObjList();
  unsigned size = objs.size();
	for (unsigned int i = 0; i < size; ++i)
	{
    if (objs[i]->MarkedForDeath())
    {
      delete objs[i];
      objs[i] = nullptr;
    }
    else if(objs[i] != nullptr)
      objs[i]->Update(dt);
	}
  Player* player = &Space::GetPlayer();
  if (player)
  {
    player->Update(dt);
  }
}

///-------------------------------------
/// Brief: Searches for and retrieves object by name
Obj* ObjManager::GetObj(const char *name)
{
	for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
	{
		if (Space::ObjList[i]->Name() == name)
			return Space::ObjList[i];
	}
	TraceMessage(TraceLog, "GOM : Object %s does not exist", name);

	return nullptr;
}

///-------------------------------------
/// Brief: Gets object at specified index
Obj* ObjManager::GetNextObj(int index)
{
	return Space::ObjList[index];
}

///-------------------------------------
/// Brief: Returns size of object array
int ObjManager::GetListSize()
{
	return Space::ObjList.size();
}

bool ObjManager::ObjIsDead(std::string& name)
{
  for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
  {
    if (Space::ObjList[i]->Name() == name)
      return Space::ObjList[i]->IsDead();
  }

  return true;
}

bool ObjManager::ObjIsActive(std::string& name)
{
  for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
  {
    if (Space::ObjList[i]->Name() == name)
      return Space::ObjList[i]->IsActive();
  }

  return true;
}

std::vector<Obj*> ObjManager::GetObjsInMap(std::string mapName)
{
  std::vector<Obj*> objs;

  for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
  {
    if (Space::ObjList[i]->MapName() == mapName)
      objs.push_back(Space::ObjList[i]);
  }

  return objs;
}

Obj * ObjManager::FindFirstDead(std::string& type)
{
  for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
  {
    if (Space::ObjList[i]->Type() == type)
      if(Space::ObjList[i]->IsDead())
        return Space::ObjList[i];
  }

  return nullptr;
}

Obj * ObjManager::FindFirstDeadofMine(eClass clss, char id)
{
  std::string type = Stats::ClassToString(clss);
  int size = Space::ObjList.size();

  for (int i = 0; i < size; ++i)
  {
    if (Space::ObjList[i]->Type() == type)
      if (Space::ObjList[i]->IsDead())
        if(Space::ObjList[i]->Name().find(id) != std::string::npos)
          return Space::ObjList[i];
  }

  return nullptr;
}

Obj * ObjManager::FindObjContaining(std::string text)
{
  for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
  {
    if (Space::ObjList[i]->Name().find(text) != std::string::npos)
      return Space::ObjList[i];
  }

  return nullptr;
}

  //gets deactivated (available for respawn/reuse) obj of type
Obj * ObjManager::GetObjCurrentScreen(std::string type)
{
  std::vector<Obj*> objs = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->CurrentObjList();
  int size = objs.size();

  for (int i = 0; i < size; ++i)
  {
    if (objs[i]->Type().compare(type) == 0)
      if (!objs[i]->IsActive())
        return objs[i];
  }

  return nullptr;
}

Obj * ObjManager::GetObjCurrScreenByName(std::string name)
{
  std::vector<Obj*> objs = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->CurrentObjList();
  int size = objs.size();

  for (int i = 0; i < size; ++i)
  {
    if (objs[i]->Name().compare(name) == 0)
    {
      bool active = objs[i]->IsActive();
      if (!active)
        return objs[i];
      else
        return nullptr;
    }
  }

  return nullptr;
}

Obj * ObjManager::GetObjThisScreen(std::string type, std::string screenName)
{
  std::vector<Obj*> objs = dynamic_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->FindScreen(screenName)->GetObjectList();
  int size = objs.size();

  for (int i = 0; i < size; ++i)
  {
    if (objs[i]->Type().compare(type) == 0)
      if (objs[i]->IsActive())
        return objs[i];
  }

  return nullptr;
}

int ObjManager::NumofEnemyInMap(eClass type, std::string mapName)
{
  int num = 0;

  for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
  {
    if (Space::ObjList[i]->MapName() == mapName)
      if (Space::ObjList[i]->Has(Stats)->Class() == type)
        ++num;
  }

  return num;
}

int ObjManager::NumDeadofTypeInMap(eClass type, std::string mapName)
{
  int num = 0;

  for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
  {
    if (Space::ObjList[i]->MapName() == mapName)
      if (Space::ObjList[i]->Has(Stats)->Class() == type)
        if(Space::ObjList[i]->IsDead())
          ++num;
  }

  return num;
}

int ObjManager::NumDeadofType(eClass type)
{
  int num = 0;

  for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
  {
    if (Space::ObjList[i]->Has(Stats) == nullptr)
      continue;

    if (Space::ObjList[i]->Has(Stats)->Class() == type)
      if (Space::ObjList[i]->IsDead())
        ++num;
  }

  return num;
}

int ObjManager::NumDeadInMap(std::string mapName)
{
  int num = 0;

  for (unsigned int i = 0; i < Space::ObjList.size(); ++i)
  {
    if (Space::ObjList[i]->MapName() == mapName)
      if (Space::ObjList[i]->IsDead())
        ++num;
  }

  return num;
}

ObjManager * ObjManager::GetInstance()
{
  return &objectManager;
}

///-------------------------------------
/// Brief: Checks for named object in object array
bool ObjManager::CheckName(const char *name, int index)
{
	if (Space::ObjList[index]->Name() == name)
		return true;
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////////

//Grid::Grid()
//{
//	//Find window width and height; calculate size of array
//	//Dynamically allocate a std::vector or an int array
//	//Initialize array to 0's
//}
//
//Grid::~Grid()
//{
//	//delete [] Grid;
//}
//
//int Grid::GetIndex(float x, float y)
//{
//	int index = 0;
//
//	index = x + (width_ * y);
//
//	return index;
//}
//
//void Grid::GetWidth()
//{
//	//Get the window pixel width and set width_
//}
//
//void Grid::GetHeight()
//{
//	//get the window pixel height and set height_
//}
//
//int Grid::GetSize()
//{
//	return size_;
//}
//
//void Grid::UpdateGrid()
//{
//
//}

Obj * ObjManagerGetObject(const char * name)
{
  return objectManager.GetObj(name);
}

void ObjManagerAddObject(Obj * obj)
{
  objectManager.AddObj(obj);
}

ObjManager & GetObjManager()
{
  return objectManager;
}

bool ObjManagerObjIsDead(std::string& name)
{
  return objectManager.ObjIsDead(name);
}

bool ObjManagerObjIsActive(std::string& name)
{
  return objectManager.ObjIsActive(name);
}

Obj * ObjManagerFindContaining(std::string text)
{
  return objectManager.FindObjContaining(text);
}
