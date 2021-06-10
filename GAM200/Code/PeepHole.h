///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: PeepHole.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  brief: This class is for the main character.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include "Obj.h"
#include "Component.h"
#include <glm/glm.hpp>
// mw - this thing has to be defined if you're going to use these last two includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

///----------------------------------GLOBALS------------------------------------

typedef enum HoleSize
{
  maxSize_,
  middleSize_,
  minSize_,
} HoleSize;
///----------------------------------CLASSES------------------------------------
class PeepHole : public Obj
{
public:
  PeepHole();
  virtual void Initialize();
  virtual void Update(float dt);
  virtual void Render();

  void UpdateSize();
  void ScaleUp();
  void ScaleDown();

  inline glm::vec2 GetAlphaCoords() { return alpha_coord; }

  inline void HoleType(HoleSize size) { holeSize_ = size; }
  inline HoleSize HoleType() { return holeSize_; }

  inline float Size() { return currSize_; }
  inline void Size(float size) { currSize_ = size; }

  inline float Max() { return max_; }
  inline void Max(float max) { max_ = max; }

  inline float Min() { return min_; }
  inline void Min(float min) { min_ = min; }

  inline float Middle() { return middle_; }
  inline void Middle(float middle) { middle_ = middle; }

private:
  glm::vec2 alpha_coord;
  HoleSize holeSize_ = maxSize_;
  float currSize_ = 2.5f;
  float max_ = 2.5f;
  float middle_ = 1.5f;
  float min_ = 1.f;
};


///---------------------------------FUNCTIONS-----------------------------------


#pragma once
