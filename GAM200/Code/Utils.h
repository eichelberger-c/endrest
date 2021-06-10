///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Utils.h
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
#include "Vec2D.h"
#include "Mesh.h"

///----------------------------------GLOBALS------------------------------------
typedef float secs;
using flag = bool;

extern float magicS; //my magic number to let me know when somethin's fishy

typedef enum LightColors
{
  NoColor_ = -1,
  Green_,
  Blue_,
  Purple_,
  Red_,
  ColorMax_,
  Yellow_, //neutral

} colorFlag;

///----------------------------------CLASSES------------------------------------
struct BoolEventData
{
  BoolEventData(std::string name = " ", bool toggle = false) : objName(name), state(toggle)
  {
  }

  std::string objName;
  bool state;
};

struct FlashData
{
  FlashData(std::string name = " ", secs dur = 5.0f) : objName(name), duration(dur)
  {
  }

  FlashData& operator=(const FlashData& rhFD)
  {
    if (objName.compare(rhFD.objName) == 0 && duration == rhFD.duration)
      return *this;

    objName.assign(rhFD.objName);
    duration = rhFD.duration;

    return *this;
  }

  std::string objName;
  secs duration;
};

struct SoundData
{
  SoundData(std::string file = " ", bool p = true) : filename(file), play(p)
  {
  }

  std::string filename;
  bool play;
};

///---------------------------------FUNCTIONS-----------------------------------
void TimeInit();
secs TotalTime();
void SpinWait(secs wait);
DWORD ToDWORD(secs seconds);
void UtilsInit();
void RandomInit();
int RandomInt(int min, int max);
float RandomFloat(float min, float max);
Vec2D RandomVec2D(float min, float max);
Vec2D RandomVec2D2(float minx, float maxx, float miny, float maxy);
float DegreesToRadians(float degrees);
float RadiansToDegrees(float radians);
glm::vec4 ColorToRBG(colorFlag color);
std::string& GetDefaultPopupString();
