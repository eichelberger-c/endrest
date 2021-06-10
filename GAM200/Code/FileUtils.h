///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: FileUtils.h
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
std::vector<std::string> GetFilesFromDirectory(std::string directory);
void CleanFileName(std::string &string);

