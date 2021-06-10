///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Win.cpp
//
//  Author: Benjamin Lantrip
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions: 
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "EndGs.h"
#include "Engine.h"
#include "GameStateManager.h"
#include "ScreenManager.h"
#include "Obj.h"
#include "TextureSys.h"
#include "Sprite.h"
#include "Transform.h"
#include "Graphics.h"

///----------------------------------GLOBALS------------------------------------

// Win/Lose Screen
static Sprite* WinOverlay = NULL;
static Sprite* WinText = NULL;
static Sprite* LoseScreen = NULL;
static Sprite* continue_ = NULL;

// Win/Lose object we will be drawing
static Obj* EGscreen = nullptr;
static Obj* EGprompt = nullptr;

// Ptr to the graphics/Texture system
static Graphics* graphics = NULL;
static TextureSys* Tsystem = NULL;

// ptr to the window
static GLFWwindow* window = NULL;

// Which Ending we are supposed to show
static Ending end = Win;

// Private Functions
static bool KeyIsPressed(void);

static float width = 1920.0f;
static float height = 1080.0f;

///----------------------------------CLASSES------------------------------------
static bool startFadeIn = false;
static Timer *daTime = nullptr;

///---------------------------------FUNCTIONS-----------------------------------

// Which ending are we supposed to load?
void SetEnding(Ending e)
{
	if (e != Invalid)
	{
		end = e;
	}
}


// Load ending screens
void EndGsLoad()
{
	// Load in the sprites
	WinOverlay = new Sprite("Data/Textures/VictoryScreenOverlay002.png");
	WinText = new Sprite("Data/Textures/VictoryText001.png");
	continue_ = new Sprite("Data/Textures/PressAnyKey.png");
	LoseScreen = new Sprite("Data/Textures/GameOverText001.png");

  float diff = 0.0f;
  WinOverlay->AdjustAlpha(diff);
  WinText->AdjustAlpha(diff);
  LoseScreen->AdjustAlpha(diff);
  continue_->AdjustAlpha(diff);

  daTime = new Timer(2.0f, CountUp_, 2.0f);
	// Get the Graphics ptr
	graphics = dynamic_cast<Graphics*>(EngineGetSystem("Graphics"));
	// get window ptr
	window = graphics->Window();
}

// Create objects to draw to the screen
void EndGsInit()
{
	EGscreen = new Obj("Screen");
	EGprompt = new Obj("Prompt");

	// Create Promt Transform
	Transform* prompt_transform = new Transform(Vec2D(0, -250));
	prompt_transform->Scale((float)graphics->GetWidth() / 2, (float)graphics->GetHeight() / 2);

	EGprompt->Add(continue_);
	EGprompt->Add(prompt_transform);

  Screen* sc = static_cast<Screen*>(static_cast<ScreenManager*>(EngineGetSystem("ScreenManager"))->CurrentScreen());

  sc->AddObject(EGprompt);
  sc->AddObject(EGscreen);
	
	switch (end)
	{
		case Win:
		{
			// Create Transform
			Transform* T1 = new Transform(Vec2D(0, 0));
			T1->Scale(width, height);

			EGscreen->Add(WinOverlay);
			EGscreen->Add(T1);

			Obj* text = new Obj("text");
			text->Add(WinText);
			text->Add(new Transform(T1));
      text->Has(Transform)->Scale(width / 2.0f, height);
      text->Has(Transform)->setDirty(true);
      text->Has(Transform)->Update(0.0f);

			EGscreen->AddSubObj(text);
			break;
		}
		case Lose:
		{
			// Create Transform
			Transform* T1 = new Transform(Vec2D(0, 200));
			T1->Scale((float)graphics->GetWidth() / 2.0f, (float)graphics->GetHeight());

			EGscreen->Add(LoseScreen);
			EGscreen->Add(T1);

			break;
		}
		default:
			break;
	}
}

void EndGsUpdate(float dt)
{
  // trying to use a timer to have the lose screen fade in
  //if (!daTime->Beep())
  //{
  //  secs time = daTime->Time();
  //  secs maxTime = daTime->MaxTime();

  //  float diff = time / maxTime;

  //  LoseScreen->AdjustAlpha(diff);
  //  continue_->AdjustAlpha(diff);

  //  daTime->Update(dt);
  //}
    
	// Update
	EGscreen->Update(dt);
	EGprompt->Update(dt);

	if (KeyIsPressed())
	{
		// Quit the Game
		dynamic_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->SetNextState(GsQuit);
	}
}

void EndGsShutdown()
{
}

void EndGsUnload()
{
	switch (end)
	{
		case Win:
			delete LoseScreen;
			break;
		case Lose:
			delete WinOverlay;
			delete WinText;
			break;
		default:
			break;
	}

  delete daTime;
  daTime = nullptr;
	//delete continue_;
	WinOverlay = NULL;
	WinText = NULL;
	LoseScreen = NULL;
}

bool KeyIsPressed(void)
{
	// Check Keys:
	// Enter
	if (glfwGetKey(window, GLFW_KEY_ENTER)) { return true; }
	// Spacebar
	if (glfwGetKey(window, GLFW_KEY_SPACE)) { return true; }
	// Escape
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) { return true; }
	// MB 1
	if (glfwGetKey(window, GLFW_MOUSE_BUTTON_1)) { return true; }
	// MB 2
	if (glfwGetKey(window, GLFW_MOUSE_BUTTON_2)) { return true; }

	// Other Alphabetical/Numerical keys
	// Numbers
	if (glfwGetKey(window, GLFW_KEY_0)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_1)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_2)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_3)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_4)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_5)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_6)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_7)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_8)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_9)) { return true; }

	// Letters
	if (glfwGetKey(window, GLFW_KEY_Q)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_W)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_E)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_R)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_T)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_Y)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_U)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_I)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_O)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_P)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_A)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_S)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_D)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_F)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_G)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_H)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_H)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_J)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_K)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_K)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_Z)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_X)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_C)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_V)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_B)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_N)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_M)) { return true; }

	// If we haven't pressed any of these keys, return false
	return false;
}
