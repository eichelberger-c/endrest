///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: ShaderSys.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: This class handles the debug components 
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "System.h"
#include "Obj.h"
#include "Spaces.h"
#include "Mesh.h"
#include "Shader.h"
///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class ShaderSys : public System
{
public:
  ShaderSys() : System("ShaderSys") {};
  ~ShaderSys() { shaders_.clear(); };
  virtual void Initialize() {};
  virtual void Update(float dt) {};

  Shader* FindShader(const std::string &vertName, std::string& fragName)
  {
    size_t size = shaders_.size();
    for (size_t i = 0; i < size; ++i)
    {
      if (shaders_[i]->fragName_ == fragName && shaders_[i]->vertName_ == vertName)
        return shaders_[i];
    }

   Shader* shade = new Shader(vertName.c_str(), fragName.c_str());
   shaders_.push_back(shade);
   return shade;
  }
private:
  std::vector<Shader*> shaders_;

  friend class Shader;
};