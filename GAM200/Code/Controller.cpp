///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Controller.cpp
//
//  Author: Thornton Stockwin
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//     -
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Controller.h"
#include <math.h>


///----------------------------------GLOBALS-----------------------------------
const double PI = 3.141592653589793;

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: default construtor
Controller::Controller() : deadZoneX_(0.12f), deadZoneY_(0.12f)
{
}

///-------------------------------------
/// Brief: Constructor
Controller::Controller(float dzX, float dzY) : deadZoneX_(dzX), deadZoneY_(dzY)
{
}

///-------------------------------------
/// Brief: Returns which controller is in use (Starts at 0 for player 1)
int Controller::GetPort()
{
  return ControllerID_ + 1;
}

///-------------------------------------
/// Brief: returns the controller state
XINPUT_GAMEPAD *Controller::GetState()
{
  return &state_.Gamepad;
}

///-------------------------------------
/// Brief: Checks for the first connected controller
///        and assigns it to the ControllerID, assigns
///        -1 to ControllerID if nothings is connected
bool Controller::CheckConnection()
{
  int dummyID = -1;

  for (DWORD i = 0; i < XUSER_MAX_COUNT && dummyID == -1; i++)
  {
    XINPUT_STATE currentState;
    ZeroMemory(&currentState, sizeof(XINPUT_STATE));

    if (XInputGetState(i, &currentState) == ERROR_SUCCESS)
    {
      dummyID = i;
    }
  }
  ControllerID_ = dummyID;

  return dummyID != -1;
}

///-------------------------------------
/// Brief: Updates the current values for
///        the JoySticks and Triggers, will
///        return false if the controller is
///        disconnected.
bool Controller::Update()
{
  if (ControllerID_ == -1)
  {
    CheckConnection();
  }

  if (ControllerID_ != -1)
  {
    ZeroMemory(&state_, sizeof(XINPUT_STATE));
    if (XInputGetState(ControllerID_, &state_) != ERROR_SUCCESS)
    {
      ControllerID_ = -1;
      return FALSE;
    }

    //////LEFT THUMB STICK////////
    float currLX = fmaxf(-1, (float)state_.Gamepad.sThumbLX / 32767);
    float currLY = fmaxf(-1, (float)state_.Gamepad.sThumbLY / 32767);

    if (abs(currLX) < deadZoneX_)
    {
      leftStickX_ = 0;
    }
    else
    {
      leftStickX_ = ((abs(currLX) - deadZoneX_) * (currLX / abs(currLX)));
    }

    if (abs(currLY) < deadZoneY_)
    {
      leftStickY_ = 0;
    }
    else
    {
      leftStickY_ = ((abs(currLY) - deadZoneY_) * (currLY / abs(currLY)));
    }

    if (deadZoneX_ > 0)
    {
      leftStickX_ *= (1 / (1 - deadZoneX_));
    }

    if (deadZoneY_ > 0)
    {
      leftStickY_ *= (1 / (1 - deadZoneY_));
    }

    //////RIGHT THUMB STICK////////
    float currRX = fmaxf(-1, (float)state_.Gamepad.sThumbRX / 32767);
    float currRY = fmaxf(-1, (float)state_.Gamepad.sThumbRY / 32767);

    if (abs(currRX) < deadZoneX_)
    {
      rightStickX_ = 0;
    }
    else
    {
      rightStickX_ = ((abs(currRX) - deadZoneX_) * (currRX / abs(currRX)));
    }

    if (abs(currRY) < deadZoneY_)
    {
      rightStickY_ = 0;
    }
    else
    {
      rightStickY_ = ((abs(currRY) - deadZoneY_) * (currRY / abs(currRY)));
    }

    if (deadZoneX_ > 0)
    {
      rightStickX_ *= (1 / (1 - deadZoneX_));
    }

    if (deadZoneY_ > 0)
    {
      rightStickY_ *= (1 / (1 - deadZoneY_));
    }

    //////Triggers////////
    leftTrigger_ = (float)state_.Gamepad.bLeftTrigger / 255;
    rightTrigger_ = (float)state_.Gamepad.bRightTrigger / 255;

    return TRUE;
  }
  return FALSE;
}

///-------------------------------------
/// Brief: Used to check if a button is being pressed

bool Controller::IsPressed(WORD button)
{
  return (state_.Gamepad.wButtons & button) != 0;
}

///-------------------------------------
/// Brief: Set Vibration amount in both motors
void Controller::SetVibration(int lMotor, int rMotor)
{
  XINPUT_VIBRATION goodVibes;
  ZeroMemory(&goodVibes, sizeof(XINPUT_VIBRATION));
  goodVibes.wLeftMotorSpeed = lMotor;
  goodVibes.wRightMotorSpeed = rMotor;
  XInputSetState(ControllerID_, &goodVibes);
}

float Controller::GetLeftAngle()
{
  float result;

  result = atan2(leftStickY_, leftStickX_);

  return result;
}

float Controller::GetRightAngle()
{
  float result;

  result = atan2(rightStickY_, rightStickX_);

  return result;
}

//Direction
StickDirection Controller::LeftStickPosition()
{
  if (leftStickX_ == 0 && leftStickY_ == 0)
  {
    return StickNull_;
  }

  float LeftRad = GetLeftAngle();

  if (LeftRad > 0.7854 && LeftRad < 2.3562)
  {
    return StickUp_;
  }

  if ((LeftRad > 2.3562 && LeftRad < 3.2) || 
           (LeftRad < -2.3562 && LeftRad > -3.2))
  {
    return StickLeft_;
  }

  if (LeftRad < -0.7854 && LeftRad > -2.3562)
  {
    return StickDown_;
  }

  if (LeftRad > -0.7854 && LeftRad < 0.7854)
  {
    return StickRight_;
  }

  return StickNull_;
}

//Use for animation direction possibly
StickDirection Controller::RightStickPosition()
{
  if (rightStickX_ == 0 && rightStickY_ == 0)
  {
    return StickNull_;
  }

  float RightRad = GetRightAngle();

  if (RightRad > 0.7854 && RightRad < 2.3562)
  {
    return StickUp_;
  }

  if ((RightRad > 2.3562 && RightRad < 3.2) ||
    (RightRad < -2.3562 && RightRad > -3.2))
  {
    return StickLeft_;
  }

  if (RightRad < -0.7854 && RightRad > -2.3562)
  {
    return StickDown_;
  }

  if (RightRad > -0.7854 && RightRad < 0.7854)
  {
    return StickRight_;
  }

  return StickNull_;
}