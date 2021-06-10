///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: BehaviorScriptTemplate.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - $itemname$
//      - $itemname$ (copy constructor)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
//#include "$itemname$.h"
#include "BehaviorScriptTemplate.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------
$itemname$::$itemname$(std::string name)
{
  Name(name);
  Start();
}

$itemname$::$itemname$($itemname$& og$itemname$)
{
  Name(og$itemname$.Name());
  //copy initialize any other variables added to h file
  //speed for example
}
