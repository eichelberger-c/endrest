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
#include "System.h"
#include "Obj.h"
#include "PushBlock.h"

///----------------------------------GLOBALS------------------------------------
typedef enum Zone {
  Nope_,
  Top_,
  Left_,
  Bottom_,
  Right_,
}Zone;

typedef enum Combinations {
  start_,
  win_,
}Combinations;

///----------------------------------CLASSES------------------------------------

class PushBlockManager : public System
{
public:
  PushBlockManager();
  ~PushBlockManager();
  virtual void Initialize();
  virtual void Update(float dt);
  void SetBlock(Obj * block, int zone);
  Obj * CheckBlock(int zone);
  void saveWinPositions(PushBlock* p);
  void saveStartPositions(PushBlock* p);
  void AddPushBlock(Obj* block);
  void AddWinBlock(Obj *block);
  void RemovePushBlock(Obj *block);
  void RemoveWinBlock(Obj *block);
  void checkSolved();
  
private:
  Obj * moveable_[5];
  int numBlocks_;
  int numWinBlocks_;
};