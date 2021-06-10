///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: GameStateTable.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Manages the states of the game. (Exciting)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateTable.h"

#include "MenuGs.h"
#include "GameGs.h"
#include "EndGs.h"
#include "TitleGs.h"

///----------------------------------GLOBALS-----------------------------------

typedef void(*GameStateFunctionPtr)();
typedef void(*UpdateFunctionPtr)(float dt);

typedef struct
{
	const char *					gameStateName;		 //the name of the gamestate 
  GameStateFunctionPtr	gameStateLoad;		 //pointer to a gamestate load function
  GameStateFunctionPtr	gameStateInit;		 //pointer to a gamestate initialization function
  UpdateFunctionPtr		  gameStateUpdate;	 //pointer to a gamestate update function
  GameStateFunctionPtr	gameStateShutdown; //pointer to a gamestate shutdown function
  GameStateFunctionPtr	gameStateUnload;	 //pointer to a gamestate unload function

} GameStatePointers;

///Not a global
static const GameStatePointers GameStateTab[GsNum] =
{
  { "MenuGs", MenuGsLoad, MenuGsInit, MenuGsUpdate, MenuGsShutdown, MenuGsUnload },
  { "TitleGs", TitleGsLoad, TitleGsInit, TitleGsUpdate, TitleGsShutdown, TitleGsUnload },
  { "GameGs", GameGsLoad, GameGsInit, GameGsUpdate, GameGsShutdown, GameGsUnload },
  { "EndGs", EndGsLoad, EndGsInit, EndGsUpdate, EndGsShutdown, EndGsUnload },
};

///---------------------------------FUNCTIONS-----------------------------------

//determine if the gamestate is valid
bool GameStateIsValid(GameStates gameState)
{
	return ((0 <= gameState) && (gameState < GsNum));
}

//determine if the gamestate is a special gamestate
bool GameStateIsSpecial(GameStates gameState)
{
	return ((gameState == GsRestart) || (gameState == GsQuit));
}

//call the Load function of the gamestate
void GameStateLoad(GameStates gameState)
{
	//check for bad variables and pointers
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateLoad) != NULL))
	{
		TraceMessage(TraceLog, "%s: Load", GameStateTab[gameState].gameStateName);
    //call the actual function
		(*GameStateTab[gameState].gameStateLoad)();
	}
}

//call the Init function for the gamestate
void GameStateInit(GameStates gameState)
{
	//check for bad variables and pointers
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateInit) != NULL))
	{
		TraceMessage(TraceLog, "%s: Init", GameStateTab[gameState].gameStateName);
    //call the actual function
		(*GameStateTab[gameState].gameStateInit)();
	}
}

//call the Update function for the gamestate
void GameStateUpdate(GameStates gameState, float dt)
{
	//check for bad variables and pointers
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateUpdate) != NULL))
	{
		//TraceMessage(false, "%s: Update", GameStateTab[gameState].gameStateName);
    //call the actual function
		(*GameStateTab[gameState].gameStateUpdate)(dt);
	}
}

//call the Shutdown function for the gamestate
void GameStateShutdown(GameStates gameState)
{
	//check for bad variables and pointers
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateShutdown) != NULL))
	{
		TraceMessage(TraceLog, "%s: Shutdown", GameStateTab[gameState].gameStateName);
    //call the actual function
		(*GameStateTab[gameState].gameStateShutdown)();
	}
}

//call the Unload function for the gamestate
void GameStateUnload(GameStates gameState)
{
	//check for bad variables and pointers
	if (GameStateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateUnload) != NULL))
	{
		TraceMessage(TraceLog, "%s: Unload", GameStateTab[gameState].gameStateName);
		//call the actual function
		(*GameStateTab[gameState].gameStateUnload)();
	}
}

