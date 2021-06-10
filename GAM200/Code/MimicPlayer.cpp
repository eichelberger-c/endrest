///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: MimicPlayer.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - MimicPlayer
//      - MimicPlayer (copy constructor)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "MimicPlayer.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
MimicPlayer::MimicPlayer(std::string name)
{
  Name(name);
  Start();
}

MimicPlayer::MimicPlayer(MimicPlayer& ogMimicPlayer)
{
  Name(ogMimicPlayer.Name());
  Start();
}

void MimicPlayer::Lag(float val)
{
  lag = val;
}
