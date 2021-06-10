///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Win.h
//
//  Author: Benjamin Lantrip
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Displays Win/Lose Screens
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

typedef enum
{
	Invalid,
	Win,
	Lose,
} Ending;

///---------------------------------FUNCTIONS-----------------------------------

// Which ending are we supposed to load?
void SetEnding(Ending e);

// Game State functions
void EndGsLoad(void);

void EndGsInit(void);

void EndGsUpdate(float dt);

void EndGsShutdown(void);

void EndGsUnload(void);