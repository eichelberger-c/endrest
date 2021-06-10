///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Area.cpp
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Area
//      - ~Area
//      - Attach(North/South/East/West)Area
//      - Detach(North/South/East/West)Area
//      - Get(North/South/East/West)Area
//      - GetPushBlockPuzzle
//      - SetPushBlockPuzzle
//      - EnemyReset
//      - PuzzleReset
//      - ResetEnemies
//      - ResetPuzzles
//      - hasPlayer
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Behavior.h"
#include "Transform.h"
#include "Physics.h"
#include "Area.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Area Constructor
Area::Area(std::string name) : Screen(name)
{
}

///-------------------------------------
/// Brief: Attaches an area to the north
void Area::AttachNorthArea(std::string name)
{
  northName_ = name;
}

///-------------------------------------
/// Brief: Attaches an area to the south
void Area::AttachSouthArea(std::string name)
{
  southName_ = name;
}

///-------------------------------------
/// Brief: Attaches an area to the east
void Area::AttachEastArea(std::string name)
{
  eastName_ = name;
}

///-------------------------------------
/// Brief: Attaches an area to the west
void Area::AttachWestArea(std::string name)
{
  westName_ = name;
}

///-------------------------------------
/// Brief: Detaches an area to the north
void Area::DetachNorthArea()
{
  northName_ = "";
}

///-------------------------------------
/// Brief: Detaches an area to the south
void Area::DetachSouthArea()
{
  southName_ = "";
}

///-------------------------------------
/// Brief: Detaches an area to the east
void Area::DetachEastArea()
{
  eastName_ = "";
}

///-------------------------------------
/// Brief: Detaches an area to the west
void Area::DetachWestArea()
{
  westName_ = "";
}

///-------------------------------------
/// Brief: returns area to the north
std::string Area::GetNorthArea()
{
  return northName_;
}

///-------------------------------------
/// Brief: returns area to the south
std::string Area::GetSouthArea()
{
  return southName_;
}

///-------------------------------------
/// Brief: returns area to the east
std::string Area::GetEastArea()
{
  return eastName_;
}

///-------------------------------------
/// Brief: returns area to the west
std::string Area::GetWestArea()
{
  return westName_;
}

///-------------------------------------
/// Brief: returns area push block puzzle
PushBlock* Area::GetPushBlockPuzzle()
{
  return pushblock_;
}

///-------------------------------------
/// Brief: sets area push block puzzle
void Area::SetPushBlockPuzzle(PushBlock * puzzle)
{
  pushblock_ = puzzle;
}

///-------------------------------------
/// Brief: sets enemy reset type
void Area::EnemyReset(reset reset)
{
  reset_.enemy = reset;
}

///-------------------------------------
/// Brief: returns enemy reset type
Area::reset Area::EnemyReset()
{
  return reset_.enemy;
}

///-------------------------------------
/// Brief: sets puzzle reset type
void Area::PuzzleReset(reset reset)
{
  reset_.puzzle = reset;
}

///-------------------------------------
/// Brief: returns puzzle reset type
Area::reset Area::PuzzleReset()
{
  return reset_.puzzle;
}

///-------------------------------------
/// Brief: resets area enemies
void Area::ResetEnemies()
{
  std::vector<Obj*>objs = GetObjectList();
  std::vector<Obj*>enemies;

  for (Obj* obj : objs)
    if (obj->ResolutionType() == resType::Enemy_)
      enemies.push_back(obj);

  if (reset_.enemy == None_)
    for (Obj* obj : enemies)
    {
      obj->Has(Physics)->Reset();
      obj->Has(Behavior)->Reset();
    }

  else if (reset_.enemy == All_)
    for (Obj *obj : enemies)
    {
      obj->ResetAll();
      obj->Resurrect();
    }

  else if (reset_.enemy == Pos_)
    for (Obj *obj : enemies)
    {
      obj->Has(Transform)->Reset();
      obj->Has(Physics)->Reset();
      obj->Has(Behavior)->Reset();
    }
}

///-------------------------------------
/// Brief: resets area puzzles
void Area::ResetPuzzles()
{
  if (!pushblock_ || reset_.puzzle == None_)
    return;

  std::vector<Obj*>blocks = pushblock_->GetPushBlocks();

  // resets all components of each pushblock
  // resets pushblock status
  if (reset_.puzzle == All_)
  {
    for (Obj* obj : blocks)
      obj->ResetAll();

    pushblock_->isSolved(false);
  }

  // resets only transform of each pushblock
  else if (reset_.puzzle == Pos_)
    for (Obj * obj : blocks)
      obj->Has(Transform)->Reset();
}



bool Area::hasPlayer()
{
  return hasPlayer_;
}

void Area::hasPlayer(bool player)
{
  hasPlayer_ = player;
}

void Area::ForceResetAll()
{
  std::vector<Obj*> objects = GetObjectList();

  for (Obj* obj : objects)
    obj->ResetAll();
}

