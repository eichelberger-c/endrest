///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: BehaviorScriptTemplate.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: A behavior "script" that //fill this in 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "BehaviorScripts.h"
#include "Player.h"
#include "Spaces.h"

///----------------------------------GLOBALS------------------------------------
enum $itemname$States
{
  ///REQUIRED:
  //some kind of Invalid state
  //at least a standard behavior state (such as Chase)

} /*a variable for the states such as scState (SlowChase State)*/;

///----------------------------------CLASSES------------------------------------
class $itemname$ : public BehaviorScript
{
public:

  $itemname$(std::string name = "$itemname$"); //always use the default DO NOT CHANGE THE NAME
  $itemname$($itemname$& og$itemname$);

  Player *play;
  Timer *tim;

  virtual void Start() override
  {
    Player& p = Space::GetPlayer();
    play = &p;
    Timer& t = Tim();
    tim = &t;
  }

  virtual void SpecialUpdate(float dt) override
  {
    if (!IsAggroed())
    {
      Vec2D pos = Parent()->Has(Transform)->Translation();
      Vec2D player = play->Has(Transform)->Translation();
      
      if (pos.IsInRange(player, AggroRadius()))
      {
        bool b = true;
        IsAggroed(b);
      }
      else
        return;
    }
  }

  virtual void Exit() override
  {

  }
};

///---------------------------------FUNCTIONS-----------------------------------


