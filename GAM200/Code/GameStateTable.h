///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: GameStateTable.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Keeps the gamestates organized, helper functions for the GSM
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------

#pragma once

typedef enum
{
	//special gamestates
	GsQuit = -3,
	GsRestart = -2,
	GsInvalid = -1,

	//normal gamestates:
  MenuGs,
  TitleGs,
  GameGs,
  EndGs,

	//other states for convenience
	GsNum,
	GsLast = GsNum - 1,

	//changeable initial gamestate 
	GsInitial = MenuGs,

} GameStates;

///---------------------------------FUNCTIONS-----------------------------------

extern bool GameStateIsValid(GameStates gameState);

extern bool GameStateIsSpecial(GameStates gameState);

extern void GameStateLoad(GameStates gameState);

extern void GameStateInit(GameStates gameState);

extern void GameStateUpdate(GameStates gameState, float dt);

extern void GameStateShutdown(GameStates gameState);

extern void GameStateUnload(GameStates gameState);

//------------------------------------------------------------------------------

