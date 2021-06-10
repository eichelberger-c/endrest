///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: MenuGs.cpp
//
//  Author: Megan Works, Benjamin Lantrip
//
//  Copyright � 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "MenuGs.h"
#include "Engine.h"
#include "GameStateManager.h"
#include "Obj.h"
#include "TextureSys.h"
#include "Sprite.h"
#include "Transform.h"
#include "Graphics.h"

///----------------------------------GLOBALS-----------------------------------
static Sprite* digipen_logo;
static Sprite* team_logo;

// Logo object we will be drawing
static Obj* DPLogo;
static Obj* TeamLogo;

// Ptr to the graphics/Texture system
static Graphics* graphics = NULL;
static TextureSys* Tsystem = NULL;

static GLFWwindow* window = NULL;

// Timer for how long logo is on screen
static float timer = 0.0f;
// Alpha, between 0-1
static float alpha = 0.0f;
static bool DP_logo = false;
static bool inLogos = true;

// Private Functions
static bool KeyIsPressed(void);

///---------------------------------FUNCTIONS-----------------------------------

void MenuGsLoad()
{
	// Load in the DigiPen and Team Logos
	digipen_logo = new Sprite("Data/Textures/Logos/DigiPen_WHITE_1024px.png");
	team_logo = new Sprite("Data/Textures/Logos/the_klang_people.png");

	// Get the Graphics ptr
	graphics = dynamic_cast<Graphics*>(EngineGetSystem("Graphics"));
	// Get the Texture System ptr;
	Tsystem = dynamic_cast<TextureSys*>(EngineGetSystem("TextureSys"));
	// get window ptr
	window = graphics->Window();
}

void MenuGsInit()
{
	// Create the Logo Objects
	DPLogo = new Obj("DPLogo");
	TeamLogo = new Obj("TeamLogo");
	// add the DP sprite
	DPLogo->Add(digipen_logo);
	TeamLogo->Add(team_logo);


	// Create Transforms
	Transform* T1 = new Transform(Vec2D(0,0));
	T1->Scale((float)graphics->GetWidth() - 150, (float)graphics->GetHeight()/2);

	Transform* T2 = new Transform(Vec2D(0,0));
	T2->Scale((float)graphics->GetWidth() / 2.0f, (float)graphics->GetHeight() - 100);
	// Add these transforms
	DPLogo->Add(T1);
	TeamLogo->Add(T2);
}

void MenuGsUpdate(float dt)
{
	// Add dt to the total
	timer += dt;
	Obj* current = (DP_logo) ? TeamLogo : DPLogo;

	current->Update(dt);

	// If we're displaying the team logo, allow for a key press to skip to the Title
	if (DP_logo && KeyIsPressed())
	{
		dynamic_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->SetNextState(TitleGs);
		inLogos = false;
		return;
	}

	// if 2 seconds total have passed; switch to team logo and start again
	if (timer >= 6.0f)
	{
		// If the team logo's already been displayed; set the next gamestate
		if (DP_logo)
		{
			dynamic_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->SetNextState(TitleGs);
			inLogos = false;
			return;
		}
		DP_logo = true;
		timer = 3;
	}

	// Draw the Logo
	graphics->Render(*current);
}

void MenuGsShutdown()
{
	// Unload the logos
	delete TeamLogo;
	delete DPLogo;
	TeamLogo = NULL;
	DPLogo = NULL;
	inLogos = false;
}

void MenuGsUnload()
{

}

bool InLogos(void)
{
	return inLogos;
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
	// MB 3
	if (glfwGetKey(window, GLFW_MOUSE_BUTTON_3)) { return true; }
	// MB 4
	if (glfwGetKey(window, GLFW_MOUSE_BUTTON_4)) { return true; }

	return false;
}