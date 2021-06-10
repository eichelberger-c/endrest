///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: PushBlock.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This class handles the push blocks used for puzzles
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "PushBlock.h"

///----------------------------------GLOBALS------------------------------------

///----------------------------------CLASSES------------------------------------


PushBlock::PushBlock()
{
}

PushBlock::~PushBlock()
{
}

void PushBlock::isSolved(bool solved)
{
  isSolved_ = solved;
}

bool PushBlock::isSolved()
{
  return isSolved_;
}

std::vector<Obj*>& PushBlock::GetPushBlocks()
{
  return pushBlocks_;
}

std::vector<Obj*>& PushBlock::GetWinBlocks()
{
  return winBlocks_;
}
