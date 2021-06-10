///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: PushBlockManager.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This class handles the push blocks used for puzzles
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "PushBlockManager.h"
#include "ScreenManager.h"
#include "Engine.h"
#include "Transform.h"

///----------------------------------GLOBALS------------------------------------
ScreenManager *screenManager;
PushBlock *puzzle;

///----------------------------------CLASSES------------------------------------


PushBlockManager::PushBlockManager() : System("PushBlockManager")
{
  
}

PushBlockManager::~PushBlockManager()
{

}

void PushBlockManager::Initialize()
{
  screenManager = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));

  numBlocks_ = 0;
  numWinBlocks_ = 0;

  for (int i = 0; i < 5; ++i)
  {
    moveable_[i] = nullptr;
  }
}

void PushBlockManager::Update(float dt)
{
  Area *currentScreen = reinterpret_cast<Area*>(screenManager->CurrentScreen());

  if (currentScreen == nullptr)
    return;

  /// NOTE FROM MEGAN:
  /// you need to check if a pushblock was returned,
  /// if puzzle is null it's because the area doesn't have any blocks
  puzzle = currentScreen->GetPushBlockPuzzle();

  if (puzzle == nullptr)
    return;

  //Reset moveable blocks array
  for (int i = 0; i < 5; ++i)
  {
    moveable_[i] = nullptr;
  }
  numBlocks_ = puzzle->pushBlocks_.size();
  numWinBlocks_ = puzzle->winBlocks_.size();

  //Check if puzzle is solved
  if (!puzzle->isSolved())
    checkSolved();
}

void PushBlockManager::SetBlock(Obj * block, int zone)
{
  moveable_[zone] = block;
}

Obj * PushBlockManager::CheckBlock(int zone)
{
  if (moveable_[zone] != nullptr)
    return moveable_[zone];
  else
    return nullptr;
}

void PushBlockManager::saveWinPositions(PushBlock *p)
{
  int num = p->winBlocks_.size();

  if (num <= 0)
    return;

  Transform *transform;
  Obj *block = nullptr;

  for (int i = 0; i < num; i++)
  {
    block = p->winBlocks_[i];
    transform = static_cast<Transform*>(block->Has(Transform));
    p->winPos_.push_back(transform->Translation());
  }
}

void PushBlockManager::saveStartPositions(PushBlock *p)
{
  int num = p->pushBlocks_.size();

  if (num <= 0)
    return;

  Transform *transform = nullptr;
  Obj *block = nullptr;

  for (int i = 0; i < num; i++)
  {
    block = p->pushBlocks_[i];
    transform = static_cast<Transform*>(block->Has(Transform));
    p->startPos_.push_back(transform->Translation());
  }
}

void PushBlockManager::AddPushBlock(Obj* block)
{
  //Create a block and increment counter
  //Get a block from editor and push its object pointer into vector
  puzzle->pushBlocks_.push_back(block);

  //Get block type and increment correct counter
  if (block->Name().find("Push") != std::string::npos)
    numBlocks_++;
}

void PushBlockManager::AddWinBlock(Obj * block)
{
  puzzle->winBlocks_.push_back(block);

  //Get block type and increment correct counter
  if (block->Name().find("Win") != std::string::npos)
    numWinBlocks_++;
}

void PushBlockManager::RemovePushBlock(Obj * block)
{
  if (puzzle)
  {
    for (int i = 0; i < numBlocks_; i++)
    {
      if (puzzle->pushBlocks_[i] == block)
      {
        delete (puzzle->pushBlocks_[i]);
        puzzle->pushBlocks_[i] = nullptr;
        numBlocks_--;
      }
    }
  }
  else
    TraceMessage(TraceLog, "There is no puzzle in this room");
}

void PushBlockManager::RemoveWinBlock(Obj * block)
{
  if (puzzle)
  {
    for (int i = 0; i < numWinBlocks_; i++)
    {
      if (puzzle->winBlocks_[i] == block)
      {
        delete (puzzle->winBlocks_[i]);
        puzzle->winBlocks_[i] = nullptr;
        numWinBlocks_--;
      }
    }
  }
  else
    TraceMessage(TraceLog, "There is no puzzle in this room");
}

void PushBlockManager::checkSolved()
{
  //Check positions of all blocks against winning positions
  Transform *transform = nullptr;
  Vec2D position;
  Vec2D winPosition;
  Obj * block = nullptr;
  int counter = 0;

  for (int i = 0; i < numBlocks_; i++)
  {
    block = puzzle->pushBlocks_[i];
    transform = static_cast<Transform*>(block->Has(Transform));
    position = transform->Translation();

    for (int i = 0; i < numWinBlocks_; i++)
    {
      winPosition = puzzle->winPos_[i];

      if (position == winPosition)
        counter++;
    }
  }

  if (counter == puzzle->winPos_.size())
    puzzle->isSolved(true);
}
