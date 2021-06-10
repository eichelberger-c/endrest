///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Screen.cpp
//
//  Author: Megan Works
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
#include "Spaces.h"
#include "Screen.h"
#include "Door.h"

///----------------------------------GLOBALS-----------------------------------
#define ASCII_A 65
#define ASCII_Z 90

static char nextID = ASCII_A;

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: screen constructor, gives name and ID
Screen::Screen(std::string name)
{
  name_ = name;
  ogName_ = name;
  ID_ = nextID;

  if (nextID == ASCII_Z)
    nextID = ASCII_A + 32;
  else if (nextID == ASCII_Z + 32)
  {
    TraceMessage(TraceLog, "Screen Constructor: That's too many screens something's wrong");
    exit(-32);
  }
  else
    nextID++;
}

std::string Screen::GetOGName()
{
  return ogName_;
}

///-------------------------------------
/// Brief: returns screen name
std::string Screen::GetName()
{
  return name_;
}

///-------------------------------------
/// Brief: sets screen name
void Screen::SetName(std::string name)
{
  name_ = name;
}

///-------------------------------------
/// Brief: returns screen type
Screen::ScreenType Screen::GetType()
{
  return type_;
}

///-------------------------------------
/// Brief: sets screen type
void Screen::SetType(ScreenType type)
{
  type_ = type;
}

///-------------------------------------
/// Brief: returns screen object list
std::vector<Obj*> Screen::GetObjectList()
{
  return objects_;
}

///-------------------------------------
/// Brief: returns screen background filename
std::string Screen::GetBackground()
{
  return background_;
}

///-------------------------------------
/// Brief: sets screen background (doesn't change until reload)
void Screen::SetBackground(std::string filename)
{
  background_ = filename;
}

///-------------------------------------
/// Brief: Creates and adds object to screen
///        give name of an object archetype (object type)
void Screen::AddNewObject(std::string archetype)
{
  int count = IncreaseObjectCount(archetype);
  std::string name = archetype + GetID() + std::to_string(count);
  Obj * obj = new Obj(*Space::GetArchetype(archetype), name);

  // adds to screen list
  //ObjManagerAddObject(obj);
  AddObject(obj);
}

///-------------------------------------
/// Brief: adds object to screen object list
void Screen::AddObject(Obj * object)
{
  objects_.push_back(object);

  // if object is door, adds behavior to door list
  if (object->Type().compare("Door") == 0)
  {
    Behavior* door = object->Has(Behavior);
    doors_.push_back(door);
  }

  ObjManagerAddObject(object);
}

///-------------------------------------
/// Brief: removes object from screen
void Screen::RemoveObject(Obj * object)
{
  std::vector<Obj*>::iterator it = std::find(objects_.begin(), objects_.end(), object);
  objects_.erase(it);

  // if object is door, removes from door list
  if (object->Type().compare("Door"))
  {
    Behavior* door = object->Has(Behavior);
    std::vector<Behavior*>::iterator dit = std::find(doors_.begin(), doors_.end(), door);
    doors_.erase(dit);
  }
}

///-------------------------------------
/// Brief: increases count of object type
///        returns count of object type
int Screen::IncreaseObjectCount(std::string type)
{
  int count = 0;

  if (objCount_.count(type) == 0)
  {
    objCount_[type] = count;
  }

  objCount_[type] = objCount_[type] + 1;

  return objCount_[type];
}

///-------------------------------------
/// Brief: returns if start screen
bool Screen::IsStartScreen()
{
  return startScreen_;
}

///-------------------------------------
/// Brief: sets as start screen
void Screen::IsStartScreen(bool start)
{
  startScreen_ = start;
}

///-------------------------------------
/// Brief: returns if all room enemies are dead
bool Screen::AllEnemiesDead()
{
  int size = objects_.size();

  for (int i = 0; i < size; ++i)
    if (objects_[i]->ResolutionType() == Enemy_)
      if (objects_[i]->IsActive())
        return false;

  return true;
}

void Screen::KillAll()
{
  int size = objects_.size();

  for (int i = 0; i < size; ++i)
    if (objects_[i]->ResolutionType() == Enemy_)
      if (objects_[i]->IsActive())
        objects_[i]->Kill();
}

///-------------------------------------
/// Brief: returns screen ID
char Screen::GetID()
{
  return ID_;
}

///-------------------------------------
/// Brief: returns door with given ID
Behavior* Screen::FindDoor(std::string doorID)
{
  for (Behavior *d : doors_)
  {
    const Door *door = dynamic_cast<Door*>(d);

    if (door->ID == doorID)
    {
      return d;
    }
  }

  return nullptr; // door not found
}
