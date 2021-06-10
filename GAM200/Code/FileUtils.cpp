///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: FileUtils.cpp
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - GetFilesFromDirectory
//      - CleanFileName
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "FileUtils.h"

///----------------------------------GLOBALS-----------------------------------

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Gets all filenames from a directory
///        returns vector of filenames
std::vector<std::string> GetFilesFromDirectory(std::string directory)
{
  std::vector<std::string>filenames; // stores all filenames in directory
  std::string filepath = directory + "/*.*"; // gets all filetypes

  WIN32_FIND_DATA data;
  HANDLE find = ::FindFirstFile(filepath.c_str(), &data);

  if (find != INVALID_HANDLE_VALUE)
  {
    while (::FindNextFile(find, &data))
    {
      if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        filenames.push_back(data.cFileName);
    }

    ::FindClose(find);
  }

  return filenames;
}


///-------------------------------------
/// Brief: Removes path and extension from filename
void CleanFileName(std::string &string)
{
  int end = string.size();
  int extension = string.find(".");

  for (int i = extension; i < end; i++)
  {
    string.pop_back();
  }

  while (string.find("/") != -1)
  {
    string.erase(string.begin());
  }
}