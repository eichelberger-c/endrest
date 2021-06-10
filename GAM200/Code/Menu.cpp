///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Menu.cpp
//
//  Author: Benjamin Lantrip
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions: Details the ImGui Main Menu for the game
//      - GameMenu
//      - PopupMenu
//		  - 2 Overloads
//      Private Functions
//      - OptionsMenu
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Graphics.h"
#include "Menu.h"
#include "Engine.h"
#include "AudioEngine.h"
#include "GameStateManager.h"
#include "Editor.h"

///----------------------------------GLOBALS-----------------------------------

// Do we show the Options menu instead of the main menu? (Default is no)
static bool show_options = false;
static bool controls = false;
static bool credits = false;

/// Private Functions ///
// Options menus
static void OptionsMenu(void);
static void ControlsMenu(void);
static void CreditsMenu(void);

// width and height of the screen
static int width;
static int height;
static int half_width;
static int half_height;

// Whether the menu is open
static bool MenuOpen = false;

// Volume values (start at 0.8f)
static int masterVolume = 80;
static int sfxVolume = 80;
static int streamVolume = 80;
static bool mute = false;

static bool canPause = false;

// FMOD LOGO SPRITE
Sprite* fmod_logo = NULL;

///---------------------------------FUNCTIONS-----------------------------------

// Main Menu for the game
void GameMenu(void)
{
	if (canPause)
	{
		// Flags for this menu
		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoTitleBar;
		flags |= ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoResize;
		flags |= ImGuiWindowFlags_NoCollapse;

		// Flags for the Modal popup menu
		ImGuiWindowFlags PopupFlags = 0;
		PopupFlags |= ImGuiWindowFlags_NoMove;
		PopupFlags |= ImGuiWindowFlags_NoResize;
		PopupFlags |= ImGuiWindowFlags_NoCollapse;
		PopupFlags |= ImGuiWindowFlags_NoTitleBar;

		width = reinterpret_cast<Graphics*>(EngineGetSystem("Graphics"))->GetWidth();
		height = reinterpret_cast<Graphics*>(EngineGetSystem("Graphics"))->GetHeight();

		half_width = width / 2;
		half_height = height / 2;

		// Show only options, not the game menu
		if (show_options)
		{
			OptionsMenu();
			return;
		}
		if (credits)
		{
			CreditsMenu();
			return;
		}

		// Otherwise, show control screen if it's open
		else if (controls)
		{
			ControlsMenu();
			return;
		}

		// Keep the window open
		bool* p_open = NULL;

		// Specify the Window Position and Size
		// To Center the window, we subtract half of the screen's width/height with the center 
		// of the screen's coordinates
		ImGui::SetNextWindowPos(ImVec2((float)half_width - 100, (float)half_height - 115), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(200, 230), ImGuiCond_Always);


		// Main Body of the Menu starts here
		if (!ImGui::Begin("Main Menu", p_open, flags))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

		// Indent Title
		ImGui::Indent(70.0f);
		ImGui::Text("ENDREST");
		ImGui::Separator();
		
		
		// Indent the buttons
		ImGui::Indent(-35.0f);

		// Start the game
		if (ImGui::Button("Resume", ImVec2(120,0)))
		{
			static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
			Menu();
		}

		ImGui::NewLine();

		// View the Controls for the game
		if (ImGui::Button("Controls", ImVec2(120, 0)))
		{
			static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
			controls = true;
		}

		ImGui::NewLine();

		// View Credits
		if (ImGui::Button("Credits", ImVec2(120, 0)))
		{
			static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
			credits = true;
		}

		ImGui::NewLine();

		// Open the Options Menu (Sound Sliders);
		if (ImGui::Button("Options", ImVec2(120, 0)))
		{
			static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
			// show the options Menu
			show_options = true;
		}

		ImGui::NewLine();

		// Exit the Game
		if (ImGui::Button("Quit", ImVec2(120, 0)))
		{
			// Open the Modal
			static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
			ImGui::OpenPopup("Quit Game?");
		}

		// If the modal popup is set to be opened
		if (ImGui::BeginPopupModal("Quit Game?", NULL, PopupFlags))
		{
			// Display Confirmation
			ImGui::Text("Are you sure you would like to quit the game?");
			ImGui::Separator();

			// if yes, exit the game
			ImGui::Indent(30.0f);
			if (ImGui::Button("Yes", ImVec2(120, 0)))
			{
				static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
				reinterpret_cast<GameStateManager*>(EngineGetSystem("GameStateManager"))->SetNextState(GsQuit);
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			// if not, close the modal
			if (ImGui::Button("No", ImVec2(120, 0)))
			{
				static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}

void Menu(void)
{
  if (canPause)
  {
    MenuOpen = !MenuOpen;
    credits = false;
    show_options = false;
    controls = false;

    // Load in the fmod logo, if it's the first time doing so
    if (fmod_logo == NULL)
    {
      fmod_logo = new Sprite("Data/Textures/Logos/fmod_logo.png");
    }
    else
    {
      delete fmod_logo;
      fmod_logo = NULL;
    }
  }
}

// Returns true if the menu is opened, otherwise false
bool MenuIsOpen(void)
{
	return MenuOpen;
}

// Dialogue for Popup Menus
void PopupMenu(std::string& text, Obj* obj, int length, int height)
{
	int height_offset = 90;
	int width_offset = 0;

	// Flags for this menu
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	int half_width = Graphics::GetWidth() / 2 - width_offset;
	int half_height = Graphics::GetHeight() / 2 - height_offset;

	// Keep the window open
	bool* p_open = NULL;

	// Location of the Popup
	ImVec2 popup = ImVec2(0, 0);
	// Object's Tranform
	Transform* transform = NULL;

	// Get object's position into an ImVec2
	// If no object is passed, assume the object is the player
	if (!obj)
	{
		Player& player = Space::GetPlayer();
		transform = reinterpret_cast<Transform*>(player.Has(Transform));
	}
	else
	{
		transform = reinterpret_cast<Transform*>(obj->Has(Transform));
	}
	
	Vec2D obj_pos = transform->Translation();
	// Popup will appear over the object
	popup.x = obj_pos.X() + half_width - length/2;
	popup.y = float(-1.0 * obj_pos.Y() + half_height - height);

	// Specify the Window Position and Size
	ImGui::SetNextWindowPos(popup, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(float(length), float(height)), ImGuiCond_Always);

	// Main Body of the Menu starts here
	if (!ImGui::Begin("Popup", p_open, flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	const char* text_c = text.c_str();

	ImGui::Text(text_c);

	ImGui::End();
}

void PopupMenu(PopupParam& params)
{
	Editor::AddPopup(params);
}

/// Private Functions ///
static void OptionsMenu(void)
{
	const char* items[] = { "Fullscreen", "Windowed"};
	static int windowType = 0;
	// Flags for this menu
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	// Keep the window open
	bool* p_open = NULL;

	// Specify the Window Position and Size3
	ImGui::SetNextWindowPos(ImVec2((float)half_width - 120, (float)half_height - 115), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(240, 230), ImGuiCond_Always);

	// Main Body of the Menu starts here
	if (!ImGui::Begin("Main Menu", p_open, flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	// Indent Title
	ImGui::Indent(80.0f);
	ImGui::Text("OPTIONS");
	ImGui::Indent(-80.0f);
	ImGui::Separator();

	// Audio Options
	AudioEngine* audio = reinterpret_cast<AudioEngine*>(EngineGetSystem("AudioEngine"));

	// Buttons
	ImGui::SliderInt("Master", &masterVolume, 0, 100);
	ImGui::NewLine();
	ImGui::SliderInt("SFX", &sfxVolume, 0, 100);
	ImGui::NewLine();
	ImGui::SliderInt("Stream", &streamVolume, 0, 100);

	// Begin mute options
	ImGui::Separator();
	ImGui::Indent(70);
	ImGui::Checkbox("Mute All", &mute);
	ImGui::Indent(15);
	if (ImGui::Button("Reset", ImVec2(60, 0)))
	{
		static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
		masterVolume = 80;
		sfxVolume = 80;
		streamVolume = 80;
		mute = false;
	}
	ImGui::Indent(-85);
	ImGui::Combo("Mode", &windowType, items, IM_ARRAYSIZE(items));

	// Set window mode
	Graphics::SetWindowMode((WindowMode)windowType);
	
	// Pass these values to the Audio Engine
	{
		// Convert int version of these volumes
		float master = (float)masterVolume / 100.0f;
		float sfx = (float)sfxVolume / 100.0f;
		float stream = (float)streamVolume / 100.0f;

		// Mute sound, if we decide to do so.
		audio->Mute(mute);

		// Set volume to:
		// Master
		audio->Volume(master);
		// Sfx
		audio->Volume(SoundType::sfx_, sfx);
		// Stream
		audio->Volume(SoundType::stream_, stream);
	}

	// Exit the Game
	//ImGui::NewLine();
	ImGui::Indent(50.0f);
	if (ImGui::Button("Back", ImVec2(120,0)))
	{
		static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
		show_options = false;
	}

	ImGui::End();
}

static void ControlsMenu(void)
{
	// Flags for this menu
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	ImGuiWindowFlags PopupFlags = 0;
	PopupFlags |= ImGuiWindowFlags_NoMove;
	PopupFlags |= ImGuiWindowFlags_NoResize;
	PopupFlags |= ImGuiWindowFlags_NoCollapse;
	PopupFlags |= ImGuiWindowFlags_NoTitleBar;

	// Keep the window open
	bool* p_open = NULL;

	// Specify the Window Position and Size
	ImGui::SetNextWindowPos(ImVec2((float)half_width - 100, (float)half_height - 115), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(200, 230), ImGuiCond_Always);

	// Main Body of the Menu starts here
	if (!ImGui::Begin("Controls", p_open, flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::Indent(64);
	ImGui::Text("Controls");
	ImGui::Separator();

	// List Controls
	ImGui::Indent(-10);
	ImGui::Text("Move Up: W");
	ImGui::Text("Move Down: S");
	ImGui::Text("Move left: A");
	ImGui::Text("Move right: D");
	ImGui::Indent(-20);
	ImGui::Text("Flash Lantern: LMB");
	ImGui::Text("Interact: Spacebar");

	for(int i = 0; i < 4; ++i)
		ImGui::NewLine();

	ImGui::Indent(30.0f);

	if (ImGui::Button("Back"))
	{
		static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
		controls = false;
	}

	ImGui::End();
}

static void CreditsMenu(void)
{
	// Flags for this menu
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	ImGuiWindowFlags PopupFlags = 0;
	PopupFlags |= ImGuiWindowFlags_NoMove;
	PopupFlags |= ImGuiWindowFlags_NoResize;
	PopupFlags |= ImGuiWindowFlags_NoCollapse;
	PopupFlags |= ImGuiWindowFlags_NoTitleBar;

	// Keep the window open
	bool* p_open = NULL;

	// Specify the Window Position and Size
	ImGui::SetNextWindowPos(ImVec2((float)half_width - 210, (float)half_height - 480), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(420, 960), ImGuiCond_Always);


	// Main Body of the Menu starts here
	if (!ImGui::Begin("Credits", p_open, flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	// Begin Formatting credits
	ImGui::Indent(170);
	ImGui::Text("Credits");
	ImGui::Separator();
	ImGui::Indent(-170);

	ImGui::Indent(100);
	ImGui::Text("Digipen Institute of Technology");
	ImGui::Indent(70);
	ImGui::Text("Presents");
	ImGui::Indent(-55);
	ImGui::Text("A Klang People production");
	ImGui::Indent(50);
	ImGui::Text("\"Endrest\"");
	ImGui::Indent(-25);
	ImGui::Text("WWW.DIGIPEN.EDU");
	ImGui::Indent(-140);
	ImGui::Text("COPYRIGHT © 2019 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.");

	// TEAM CREDITS
	ImGui::Indent(165);
	ImGui::Text("Developed By");
	ImGui::Indent(-35);
	// Director, Producer, and Leads
	ImGui::Text("DIRECTOR AND PROGRAMMER");
	ImGui::Indent(35);
	ImGui::Text("Sonya Toft");
	ImGui::Indent(-35);
	ImGui::Text("PRODUCER AND PROGRAMMER");
	ImGui::Indent(20);
	ImGui::Text("Zachary Keenly");
	ImGui::Indent(10);
	ImGui::Text("DESIGN LEAD");
	ImGui::Indent(-10);
	ImGui::Text("Isabelle Farmer");
	//ImGui::Indent(10);
	ImGui::Text("TECHNICAL LEAD");
	ImGui::Indent(10);
	ImGui::Text("Megan Works");
	// Designers
	ImGui::Text("UX DESIGNER");
	ImGui::Indent(-5);
	ImGui::Text("Cameron Bowman");
	ImGui::Indent(-15);
	// Programmers
	ImGui::Text("GRPAHICS PROGRAMMER");
	ImGui::Text("Charles Eichelberger");
	ImGui::Indent(10);
	ImGui::Text("TOOLS PROGRAMMER");
	ImGui::Indent(-5);
	ImGui::Text("Benjamin Lantrip");
	// GAM200/250 Instructors
	ImGui::Indent(10);
	ImGui::Text("INSTRUCTORS");
	ImGui::Text("Ellen Beeman");
	ImGui::Text("Tacket Brown");
	ImGui::Text("Matt Brunner");
	ImGui::Text("Jeremy Holcomb");
	ImGui::Indent(-15);
	ImGui::Text("Rachel Rutherford");
	ImGui::Indent(15);
	ImGui::Text("Steven Saulls");
	ImGui::Text("Doug Schilling");
	ImGui::Indent(-10);
	ImGui::Text("Lawrence Schwedler");
	ImGui::Indent(10);
	ImGui::Text("Nick Wiswell");
	ImGui::NewLine();
	// Digipen Executives
	ImGui::Indent(10);
	ImGui::Text("PRESIDENT");
	ImGui::Indent(-10);
	ImGui::Text("Claude Comair");
	ImGui::Indent(10);
	ImGui::Text("EXECUTIVES");
	ImGui::Indent(-40);
	ImGui::Text("Jason Chu  John Bauer");
	ImGui::Indent(-20);
	ImGui::Text("Samir Abu Samra  Raymond Yan");
	ImGui::Indent(-10);
	ImGui::Text("Prasanna Ghali  Michele Comair");
	ImGui::Indent(40);
	ImGui::Text("Xin Li  Angela Kugler");
	ImGui::Indent(-55);
	ImGui::Text("Melvin Gonzalvez  Meighan McKelvey");

	// Special Thanks
	ImGui::Indent(65);
	ImGui::Text("SPECIAL THANKS");
	ImGui::Indent(-5);
	// Art Lead
	ImGui::Text("Jacob Saunderson");
	ImGui::Indent(5);
	// Character Artist
	ImGui::Text("Lily Benardout");
	ImGui::Indent(5);
	// Animation Artist
	ImGui::Text("Chris Shell");
	ImGui::Indent(-10);
	// Programmer
	ImGui::Text("Thornton Stockwin");

	// Software Logos
	// FMOD Logo
	if (fmod_logo)
	{
		// for our engine, a ImTextureID is a GLuint
		unsigned int texId = fmod_logo->GetSpriteSource()->GetId();
		ImTextureID id = (void*)texId;
		float width = (float)fmod_logo->GetImgWidth();
		float height = (float)fmod_logo->GetImgHeight();

		ImGui::NewLine();
		ImGui::Indent(-20);
		ImGui::Text("Developed with the help of");
		ImGui::Indent(20);
		// Display the image
		ImGui::Indent(-60);
		ImGui::Image(id, ImVec2(240, 80), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
		ImGui::Indent(60);
	}

	if (ImGui::Button("Back", ImVec2(120,0)))
	{
		static_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Play("Data/Audio/MenuClick.wav");
		credits = false;
	}

	ImGui::End();
}

void CanPause(bool b)
{
	canPause = b;
}
	//if(MenuOpen)
	//	reinterpret_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Pause(true);
	//else
	//	reinterpret_cast<AudioEngine*>(EngineGetSystem("AudioEngine"))->Pause(false);