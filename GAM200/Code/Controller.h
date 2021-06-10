///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Controller.h
//
//  Author: Thornton Stockwin
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This class handles the Controller.
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "System.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <xinput.h>

///----------------------------------GLOBALS------------------------------------
enum StickDirection {
  StickNull_,
  StickUp_,
  StickLeft_,
  StickDown_,
  StickRight_,
};

///----------------------------------CLASSES------------------------------------#pragma once
class Controller
{
public:
  Controller();
  Controller(float dzX, float dzY);

  int GetPort();
  XINPUT_GAMEPAD *GetState();

  bool CheckConnection();
  bool Update();
  bool IsPressed(WORD);

  void SetVibration(int lMotor = 0, int Rmotor = 0);

  float GetLeftAngle();
  float GetRightAngle();

  StickDirection LeftStickPosition();
  StickDirection RightStickPosition();


  float leftStickX_;
  float leftStickY_;

  float rightStickX_;
  float rightStickY_;

  float leftTrigger_;
  float rightTrigger_;

private:
  //which controller is connected(0 = player 1, ect)
  int ControllerID_;

  XINPUT_STATE state_;
  float deadZoneX_;
  float deadZoneY_;


};