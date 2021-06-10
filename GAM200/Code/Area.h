///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Area.h
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: area game screens
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Screen.h"
#include "PushBlock.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class Area : public Screen
{
public:
  Area(std::string name);
  ~Area() {};

  // reset types, default is that nothing resets
  typedef enum
  {
    None_,           // only basic stuff resets (physics, isaggroed)
    All_,            // reset everything (i.e. unkill, reset puzzle status)
    Pos_,            // reset just positions
  }reset;

  typedef struct ResetType
  {
    reset enemy = None_;  // controls how enemies reset
    reset puzzle = None_; // controls how puzzles reset
    // reset events = None_; // controls how/if events are reset
  } resetType;

  //void SetDoor(DoorData& data);

  void AttachNorthArea(std::string name);
  void AttachSouthArea(std::string name);
  void AttachEastArea(std::string name);
  void AttachWestArea(std::string name);

  void DetachNorthArea();
  void DetachSouthArea();
  void DetachEastArea();
  void DetachWestArea();
  
  std::string GetNorthArea();
  std::string GetSouthArea();
  std::string GetEastArea();
  std::string GetWestArea();

  PushBlock* GetPushBlockPuzzle();
  void SetPushBlockPuzzle(PushBlock* puzzle);

  void EnemyReset(reset reset);
  reset EnemyReset();

  void PuzzleReset(reset reset);
  reset PuzzleReset();

  // does the resetting
  void ResetEnemies();
  void ResetPuzzles();

  // for serialization
  bool hasPlayer();
  void hasPlayer(bool player);

  // for game reset
  void ForceResetAll();

private: 
  resetType reset_;     // area reset type

  PushBlock * pushblock_ = nullptr; 

  bool hasPlayer_ = false;

  std::string northName_ = "";
  std::string southName_ = "";
  std::string eastName_ = "";
  std::string westName_ = "";
};

///---------------------------------FUNCTIONS-----------------------------------


