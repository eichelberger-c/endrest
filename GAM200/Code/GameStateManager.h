///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: GameStateManager.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "GameStateTable.h"
#include "System.h"

///----------------------------------GLOBALS-----------------------------------

///----------------------------------CLASSES------------------------------------
class GameStateManager : public System
{
public:
  typedef struct
  {
  	GameStates	current;	/* Index of the current game state. */
  	GameStates	previous;	/* Index of the previous game state. */
  	GameStates	next;		/* Index of the next game state. */
  
  } GameState;
  GameStateManager() : System("GameStateManager") {}
  virtual void Initialize();
  virtual void Update(float dt);

  bool StateIsRestarting();
  bool ManagerIsRunning();
  void SetNextState(GameStates nextState);

  std::map<int, std::string> stateNames_;

  GameStates GetCurrentGameState(void) const;

private:
  bool StateIsChanging() { return (gameState_.current != gameState_.next); }
  GameState gameState_;
};

///---------------------------------FUNCTIONS-----------------------------------


//------------------------------------------------------------------------------
