
///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: PushBlock.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This class handles the push blocks used for puzzles
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "Obj.h"
#include "Vec2D.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class PushBlock
{
public:
  PushBlock();
  ~PushBlock();
  void isSolved(bool solved);
  bool isSolved();
  std::vector<Obj*>& GetPushBlocks();
  std::vector<Obj*>& GetWinBlocks();

  friend class PushBlockManager;

private:
  bool isSolved_ = false;
  std::vector<Vec2D>startPos_;
  std::vector<Vec2D>winPos_;
  std::vector<Obj*>pushBlocks_;
  std::vector<Obj*>winBlocks_;
};