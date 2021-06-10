///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: CallBacks.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Moved callback functions to this file so they may be called easily
//         for opengl in graphics, more can be added for ease later.
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Graphics.h"

#include <GLFW/glfw3.h>

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------


///---------------------------------FUNCTIONS-----------------------------------
void InputCB(GLFWwindow *window, int key, int scancode, int action, int mod);

void MouseScrollCB(GLFWwindow *pwin, double xoffset, double yoffset);

void FramebufferSizeCB(GLFWwindow* window, int width, int height);

void MouseButtonCB(GLFWwindow *window, int button, int action, int mod);

void cursorEnterCB(GLFWwindow *window, int in);