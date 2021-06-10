///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: FrameRateController.h
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
#include <chrono>
#include "Utils.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

typedef struct FrameData
{
  int framesPassed;
  secs totalFrameTime;

  secs shortestFrame;
  secs longestFrame;
  secs averageFrame;

} FrameData;

class FrameRateManager
{
public:
  FrameRateManager(float targetFPS = 60.0f);
  void TargetFPS(float targetFPS);
  float TargetFPS() const;
  secs FrameDuration() const;
  float EnforceFPS();
  
  // added by cj
  float GetFPS() { return currentFPS_; }
  void ReportFrameStats() const;

  FrameData *frameStats;

private:
  secs frameStart_;
  secs frameEnd_;
  secs frameDuration_;

  float targetFPS_;
  float currentFPS_;

  secs totalDiff_;
  secs waitDuration_;
  secs sleepDuration_;

  static const float MINFPS_;
  static const float MAXFPS_;

  float mindt_ = 1 / MAXFPS_;
  float maxdt_ = 1 / MINFPS_;

  void UpdateFrameData_();
};


///---------------------------------FUNCTIONS-----------------------------------


