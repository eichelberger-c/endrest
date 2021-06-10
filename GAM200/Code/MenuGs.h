///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: MenuGs.h
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------


///---------------------------------FUNCTIONS-----------------------------------

void MenuGsLoad();

void MenuGsInit();

void MenuGsUpdate(float dt);

void MenuGsShutdown();

void MenuGsUnload();

// Tells whether we are in logos or not
bool InLogos(void);