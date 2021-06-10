///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Collect.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Collect
//      - Collect (copy constructor)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Collect.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
Collect::Collect(std::string name)
{
  Name(name);
  Start();
}

Collect::Collect(Collect& ogCollect)
{
  Name(ogCollect.Name());
  Start();
  //copy initialize any other variables added to h file
  //speed for example
}
