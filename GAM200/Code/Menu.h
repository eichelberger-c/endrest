///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Menu.h
//
//  Author: Benjamin Lantrip
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Declares functions which display certain ImGui menu options,
//         Including Mainmenu, Options Menu, and Popup menus
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include <GLFW\glfw3.h>
#include "ImGuiafx.h"
#include "Spaces.h"
#include "Inspection.h"
#include "MenuGs.h"

///----------------------------------GLOBALS------------------------------------
typedef class Obj Obj;

///----------------------------------CLASSES------------------------------------

//PopupParam moved to Inspection.h by Sonya

///---------------------------------FUNCTIONS-----------------------------------
// Main Menu
void GameMenu(void);

// Open/Close the Menu
void Menu(void);

// Returns true if the menu is opened, otherwise false
bool MenuIsOpen();

// "Menu" (textbox) that pops up when player interacts with something
void PopupMenu(std::string& text, Obj* obj = NULL, int length = 100, int height = 25);

// Event System Overload; takes a struct that has the original parameters
void PopupMenu(PopupParam& params);

// Is the Menu supposed to Open?
void CanPause(bool b);