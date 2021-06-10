///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Font.h
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
class Font
{
public:
  Font() {};
  ~Font();

  void Initialize(std::string filename);
  void Print();

  void Test();

private:
  unsigned char *buffer_;
  unsigned char *bitmap_;

};

///---------------------------------FUNCTIONS-----------------------------------


