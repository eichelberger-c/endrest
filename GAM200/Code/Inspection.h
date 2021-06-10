///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Inspection.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "EventComp.h"
#include "Spawner.h"
#include "Engine.h"
#include "Utils.h"

///----------------------------------GLOBALS------------------------------------
struct PopupParam
{
  PopupParam(std::string& text_, Obj* obj_ = NULL, int length_ = 100, int height_ = 25, float time_ = 2.0f)
    : text(text_), obj(obj_), length(length_), height(height_), time(time_) {}

  //Added by Sonya - I need a default constructor for the templates
  PopupParam() : text(GetDefaultPopupString()), obj(NULL), length(100), height(25), time(2.0f)
  {
  }

  //Also added by Sonya - to make copying easier
  PopupParam& operator=(const PopupParam& rhPP)
  {
    text = rhPP.text;
    obj = rhPP.obj;
    length = rhPP.length;
    height = rhPP.height;
	  time = rhPP.time;

    return *this;
  }

  std::string text;
  Obj* obj;
  int length;
  int height;
  float time;
};

///----------------------------------CLASSES------------------------------------
class Inspection : public EventComp<PopupParam>
{
public:
  Inspection(trigType type, PopupParam& data);
  Inspection(PopupParam& data, std::initializer_list<std::string> rooms = { EngineGetCurrentScreen() }, trigType type = OnRoomsComplete_);
  Inspection(PopupParam& data, resType collisionT = Player_, trigType type = OnCollision_);
  Inspection(Inspection& ogInspection);

  void SetData(PopupParam& data);
  std::string Text();
  std::string ObjName();
  int Length();
  int Height();
  float Time();

private:

};

///---------------------------------FUNCTIONS-----------------------------------


