///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: TitleGs.cpp
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
#include "TitleGs.h"
#include "Engine.h"
#include "GameStateManager.h"
#include "Obj.h"
#include "TextureSys.h"
#include "Sprite.h"
#include "Transform.h"
#include "Graphics.h"

///----------------------------------GLOBALS------------------------------------

static Sprite* title_card;
static Sprite* continue_;

// objects we will be drawing
static Obj* Title;
static Obj* prompt;

// Ptr to the graphics/Texture system
static Graphics* graphics = NULL;
static TextureSys* Tsystem = NULL;

// ptr to the window
static GLFWwindow* window = NULL;

static bool firstTime = true;
static float timer = 0.0f;

// Private Functions
static bool KeyIsPressed(void);

///----------------------------------CLASSES------------------------------------


///---------------------------------FUNCTIONS-----------------------------------



// Load ending screens
void TitleGsLoad()
{
	// Load in the sprites
	title_card = new Sprite("Data/Textures/title_screen.png");
	continue_ = new Sprite("Data/Textures/PressAnyKey.png");

	// Get the Graphics ptr
	graphics = dynamic_cast<Graphics*>(EngineGetSystem("Graphics"));
	// get window ptr
	window = graphics->Window();
}

// Create objects to draw to the screen
void TitleGsInit()
{
	Title = new Obj("Title");
	prompt = new Obj("Prompt");

	// Create Promt Transform
	Transform* prompt_transform = new Transform(Vec2D(0, -250));
	prompt_transform->Scale((float)graphics->GetWidth()/2, (float)graphics->GetHeight()/2);

	Transform* T1 = new Transform(Vec2D(0, 0));
	T1->Scale((float)graphics->GetWidth(), (float)graphics->GetHeight()+100);
	// Add the components to the objects
	Title->Add(title_card);
	prompt->Add(continue_);
	Title->Add(T1);
	prompt->Add(prompt_transform);
}

void TitleGsUpdate(float dt)
{
	// Update
	Title->Update(dt);
	prompt->Update(dt);

	if (KeyIsPressed() && !firstTime)
	{
		// Quit the Game
		dynamic_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->SetNextState(GameGs);
	}

	// Render
	Title->Render();
	prompt->Render();

	timer += dt;
	if(timer > 1.0f)
		firstTime = false;
}

void TitleGsShutdown()
{
	delete Title;
	delete prompt;
}

void TitleGsUnload()
{

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
