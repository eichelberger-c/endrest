///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: FrameRateController.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - FrameRateCounter
//      - Count
//      - FPS
//      - FrameRateManager
//      - TargetFPS
//      - 
//      - 
//      - 
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#pragma comment(lib, "winmm.lib")
#include "FrameRateController.h"

#include <ctime>

///----------------------------------GLOBALS-----------------------------------
FrameData stats;

const float FrameRateManager::MAXFPS_ = 60.0f;
const float FrameRateManager::MINFPS_ = 30.0f;
///---------------------------------FUNCTIONS-----------------------------------

FrameRateManager::FrameRateManager(float targetFPS)
{
  TargetFPS(targetFPS);
  
  currentFPS_ = 0.0;
  sleepDuration_ = 0.0;
  frameStart_ = TotalTime();
  frameEnd_ = frameStart_ - 1;
  frameDuration_ = 1;

  frameStats = &stats;
  stats.shortestFrame = 1.0;
  stats.longestFrame = 0.0;
  stats.averageFrame = 0.0;
}

void FrameRateManager::TargetFPS(float targetFPS)
{
  if (targetFPS < MINFPS_)
    targetFPS = MINFPS_;
  if (targetFPS > MAXFPS_)
    targetFPS = MAXFPS_;

  targetFPS_ = targetFPS;
}

float FrameRateManager::TargetFPS() const
{
  return targetFPS_;
}

secs FrameRateManager::FrameDuration() const
{
  return frameDuration_;
}

float FrameRateManager::EnforceFPS()
{
  /*frameEnd_ = TotalTime();

  frameDuration_ = frameEnd_ - frameStart_; 

  if ((frameEnd_ - lastReport_) > 1.0f)
  {
    lastReport_ = frameEnd_;
    currentFPS_ = (float)frameCount_ / 1.0f;
    frameCount_ = 1;
  }
  else
  {
    ++frameCount_;
    ++stats.framesPassed;
    UpdateFrameData_();
  }

  sleepDuration_ = targetDuration_ - frameDuration_;
  if (sleepDuration_ > 0.0)
    SpinWait(targetDuration_ - frameDuration_);

  return frameDuration_ + (frameStart_ - frameEnd_);*/

  frameStart_ = TotalTime();
  frameDuration_ = frameStart_ - frameEnd_; //subtract end from start to get non-negative number

  totalDiff_ = mindt_ - frameDuration_; //get total difference between desired frame length and actual length
  if (totalDiff_ > 0.0f)
  {
    sleepDuration_ = totalDiff_ * 0.9f;
    waitDuration_ = totalDiff_ - sleepDuration_;

    timeBeginPeriod(1);
    Sleep(ToDWORD(sleepDuration_));
    SpinWait(waitDuration_);
  }
  ++stats.framesPassed;
  UpdateFrameData_();
  currentFPS_ = 1.0f / (frameDuration_ + totalDiff_);

  return frameDuration_ + totalDiff_;
}

void FrameRateManager::ReportFrameStats() const
{
  TraceMessage(TraceLog, "FrameRateController Exit Report:\nShortest: %.10f\nLongest:  %.10f\nAverage:  %.10f", stats.shortestFrame, stats.longestFrame, stats.averageFrame);
}

void FrameRateManager::UpdateFrameData_()
{
  float thisFrame = frameDuration_ + totalDiff_;

  stats.totalFrameTime += thisFrame;

  if (thisFrame > stats.longestFrame)
    stats.longestFrame = thisFrame;
  if (thisFrame < stats.shortestFrame)
    stats.shortestFrame = thisFrame;

  stats.averageFrame = stats.totalFrameTime / stats.framesPassed;
}
