///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Callbacks.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - FramebufferSizeCB
//      - ProcessInput
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
///----------------------------------INCLUDES-----------------------------------
//Extentions in use for graphics
#include "stdafx.h"
#include "CallBacks.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Spaces.h"
#include "Engine.h"
#include "GameStateManager.h"
#include "Camera.h"
#include "DebugSystem.h"
#include "Input.h"
#include "Lighting.h"
#include "FrameRateController.h"
#include "PeepHole.h"
#include "Editor.h"
#include "Menu.h"
#include "MenuGs.h"
#include "EndGs.h"
///----------------------------------GLOBALS------------------------------------;
extern FrameRateManager FPSController;
///---------------------------------FUNCTIONS-----------------------------------
///------------------------------------------
///Author: Thornton Stockwin
///Brief: Callback function to change the size of the viewport in window
void FramebufferSizeCB(GLFWwindow* window, int width, int height)
{
 /* GLfloat aspect = (GLfloat)width / (GLfloat)height;

  glm::mat4 projection = glm::ortho(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);


  glLoadIdentity();*/
}

void InputCB(GLFWwindow *window, int key, int scancode, int action, int mod)
{
  DebugSystem* debug = reinterpret_cast<DebugSystem*>(EngineGetSystem("DebugSystem"));
  Editor* editor = reinterpret_cast<Editor*>(EngineGetSystem("Editor"));

  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
  {
#ifdef _DEBUG
    dynamic_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->SetNextState(GsQuit);
    Editor::Paused();
#endif // DEBUG


  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && !InLogos())
  {
    GLFWwindow *window = Graphics::Window();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      //dynamic_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->SetNextState(GsQuit);
	  //Editor::Paused();
		Menu();
    }
  }

  if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
  {
    editor->SetActive(!editor->GetActive());
  }
  if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
  {
    debug->ChangeMode(true);
  }

  static bool check1 = true;
  if (key == GLFW_KEY_F4 && action == GLFW_PRESS)
  {
    // to end room
    ScreenManager* scn = static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"));
    scn->ChangeCurrentScreen("TheChamber");

    // press again to kill boss
    if (EngineGetCurrentScreen().compare("TheChamber") == 0)
    {
      if (check1)
      {

        Obj* nec = ObjManagerGetObject("NecromancerL1");
        bool invince = false;
        nec->Has(Health)->Invincible(invince);
        nec->Has(Health)->HitPoints(0);
      }
    }
  }
  if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
  {
    Health* health = Space::GetPlayer().Has(Health);
    bool vince = !health->Invincible();
    health->Invincible(vince);
  }
  if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
  {
    Space::GetPlayer().Has(Health)->HitPoints(0);
  }
  if (editor->GetActive())
  {
	  if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
	  {
		  float zoom = Camera::GetCamera()->GetZoom();
		  zoom -= .5f;
		  if (zoom <= 0)
			  zoom = .1f;
		  Camera::GetCamera()->SetZoom(zoom);
	  }

	  if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
	  {

		  float zoom = Camera::GetCamera()->GetZoom();
		  zoom += .5f;
		  if (zoom >= 10)
			  zoom = 10;
		  Camera::GetCamera()->SetZoom(zoom);
	  }
	  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	  {
		  editor->Copy();
	  }

	  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	  {
		  editor->Paste();
	  }
	  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	  {
		  editor->SetSave(!editor->GetSave());
	  }
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
    {
		editor->SetDelete(!editor->GetDelete());
    }
  }


  if (key == GLFW_KEY_F7 && action == GLFW_PRESS)
  {
    Obj* temp = Space::GetPlayer().GetPeepHole();
    bool b = !temp->IsActive();
    temp->IsActive(b);
    Lighting* light = Space::GetPlayer().Has(Lighting);
    light->Toggle(!light->Toggle());
  }

  if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
  {
	  WindowMode mode = Graphics::GetWindowMode();
	  if (mode == FullScreen)
		  mode = Maximized;
	  else
		  mode = FullScreen;
	  Graphics::SetWindowMode(mode);
  }

  if (key == GLFW_KEY_P && action == GLFW_PRESS)
  {
    
  }
}

void MouseScrollCB(GLFWwindow *window, double xoffset, double yoffset)
{
  if (reinterpret_cast<Editor*>(EngineGetSystem("Editor"))->GetActive())
  {
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
      float zoom = Camera::GetCamera()->GetZoom();
      zoom += float(yoffset);
      if (zoom >= 10)
        zoom = 10;
      if (zoom <= 0)
        zoom = .1f;
      Camera::GetCamera()->SetZoom(zoom);
    }
  }
}

void MouseButtonCB(GLFWwindow *window, int button, int action, int mod)
{
  Editor* editor = reinterpret_cast<Editor*>(EngineGetSystem("Editor"));
  if (editor->GetActive())
  {
    if (!ImGui::GetIO().WantCaptureMouse)
    {
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        reinterpret_cast<Editor*>(EngineGetSystem("Editor"))->SetSelectedObject();
    }
  }
  else
  {
    Input* input = reinterpret_cast<Input*>(EngineGetSystem("Input"));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
		input->attackReady(true);
    }
  }
}