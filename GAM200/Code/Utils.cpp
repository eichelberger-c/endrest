///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Utils.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#define _USE_MATH_DEFINES

#include "stdafx.h"
#include "Utils.h"
#include "Winbase.h"
#include <ctime>
#include "Stats.h"
#include "Vec2D.h"

///----------------------------------GLOBALS-----------------------------------
clock_t start; //variable to store game start time
float magicS;
std::string popupDefault("Spooky");

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: converts clock_t milliseconds to Klang seconds
secs ToSecs(clock_t millisecs)
{
  return (secs)(millisecs / CLOCKS_PER_SEC);
}

///-------------------------------------
/// Brief: converts from Klang seconds to clock_t milliseconds
clock_t FromSecs(secs seconds)
{
  return (clock_t)(seconds * CLOCKS_PER_SEC);
}

///-------------------------------------
/// Brief: converts from Klng seconds to DWORDs
DWORD ToDWORD(secs seconds)
{
  return (DWORD)(seconds * CLOCKS_PER_SEC);
}

void UtilsInit()
{
  TimeInit();
  Stats::StatsInit();
  RandomInit();
  magicS = 420.0f;
}

///-------------------------------------
/// Brief: gets the start time
void TimeInit()
{
  start = clock();
}

///-------------------------------------
/// Brief: 
secs TotalTime()
{
  clock_t now = clock();

  return ToSecs(now - start);
}

///-------------------------------------
/// Brief: 
void SpinWait(secs wait)
{
  clock_t end = FromSecs(wait) + clock();

  while (end > clock())
  {
    //wait like a good child
  }
}

void RandomInit()
{
  srand(clock());
}

int RandomInt(int min, int max)
{
  return (rand() / (RAND_MAX + 1) * ((max + 1) - min) + min);
}

float RandomFloat(float min, float max)
{
  return (float)((double)rand() / (RAND_MAX + 1) * ((max + 1) - min) + min);
}

Vec2D RandomVec2D(float min, float max)
{
  float x = RandomFloat(min, max);
  float y = RandomFloat(min, max);

  return Vec2D(x, y);
}

Vec2D RandomVec2D2(float minx, float maxx, float miny, float maxy)
{
  float x = RandomFloat(minx, maxx);
  float y = RandomFloat(miny, maxy);

  return Vec2D(x, y);
}

float DegreesToRadians(float degrees)
{
  return (float)(degrees * (M_PI / 180.0));
}

float RadiansToDegrees(float radians)
{
  return (float)(radians * (180.0 / M_PI));
}

glm::vec4 ColorToRBG(colorFlag color)
{
  glm::vec4 col;
  col[0] = 1.0f;
  col[1] = 1.0f;
  col[2] = 1.0f;
  col[3] = 1.0f;

  switch (color)
  {
  case Green_:
    col[2] = 2.0f;
    break;
  case Blue_:
    col[1] = 2.0f;
    break;
  case Purple_:
    col[0] = 2.0f;
    col[1] = 2.0f;
    break;
  case Red_:
    col[0] = 2.0f;
    break;
  default: //default leaves it Yellow_ (neutral/no color)
    break;
  }

  return col;
}

std::string & GetDefaultPopupString()
{
  return popupDefault;
}


