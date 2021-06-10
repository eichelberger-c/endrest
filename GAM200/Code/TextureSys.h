///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: TextureSys.h
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
#include "SpriteSource.h"
///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class TextureSys : public System
{
public:
  TextureSys() : System("TextureSys") {};
  ~TextureSys() { sources_.clear(); meshes_.clear(); };
  virtual void Initialize() {};
  virtual void Update(float dt) {};

  SpriteSource* FindSource(const std::string &name, unsigned int rows, unsigned int cols, bool hole = false);
  Mesh* FindMesh(unsigned x, unsigned y, MeshType typeMesh);
private:
  std::vector<SpriteSource*> sources_;
  std::vector<Mesh*> meshes_;
  
  
  friend class SpriteSource;
  friend class Mesh;
};