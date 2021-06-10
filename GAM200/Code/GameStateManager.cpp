///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: GameStateManager.cpp
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
#include "System.h"
#include "GameStateTable.h"
#include "GameStateManager.h"


///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

void GameStateManager::Initialize()
{
  TraceMessage(TraceLog, "GSM: Initialize");

  gameState_.previous = GsInvalid;
  gameState_.current = GsInvalid;
  gameState_.next = GsInitial;

  stateNames_[GsQuit] = "GsQuit";
  stateNames_[GsRestart] = "GsRestart";
  stateNames_[GsInvalid] = "GsInvalid";
  //stateNames_[MilestoneGameState] = "MilestoneGameState";
  stateNames_[MenuGs] = "MenuGs";
  stateNames_[GameGs] = "GameGs";
  stateNames_[EndGs] = "EndGs";
  stateNames_[TitleGs] = "TitleGs";
  stateNames_[GsNum] = "GsNum";
  stateNames_[GsLast] = "GsLast";
  stateNames_[GsInitial] = "GsInitial";
}

void GameStateManager::Update(float dt)
{
  //TraceMessage(false, "GSM: Update");

  // Check for a game state change
  if (StateIsChanging())
  {
    // Shutdown the current game state
    GameStateShutdown(gameState_.current);

    if (gameState_.next == GsRestart)
      gameState_.next = gameState_.current;
    else
    {
      // TODO: Implement code to properly handle unloading the current game state
      GameStateUnload(gameState_.current);

      // Update the recorded states.
      gameState_.previous = gameState_.current;
      gameState_.current = gameState_.next;

      // TODO: Implement code to properly handle loading the new game state
      GameStateLoad(gameState_.current);
    }

    // Initialize the new game state
    GameStateInit(gameState_.current);
  }

  // Update the current game state
  GameStateUpdate(gameState_.current, dt);
}

bool GameStateManager::StateIsRestarting()
{
  return gameState_.next == GsRestart;
}

bool GameStateManager::ManagerIsRunning()
{
  return gameState_.current != GsQuit;
}

void GameStateManager::SetNextState(GameStates nextState)
{
  if (GameStateIsValid(nextState) || GameStateIsSpecial(nextState))
  {
    gameState_.next = nextState;
    TraceMessage(TraceLog, "Set next state to: %s", stateNames_[nextState].c_str());
  }
  else
    TraceMessage(TraceLog, "Tried to set invalid state: %d", nextState);
}

GameStates GameStateManager::GetCurrentGameState(void) const
{
	return gameState_.current;
}