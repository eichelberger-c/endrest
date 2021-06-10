///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: TextureSys.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Does some texture stuff 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

#include "TextureSys.h"

SpriteSource * TextureSys::FindSource(const std::string & name, unsigned int rows, unsigned int cols, bool hole)
{
  unsigned int size = sources_.size();
  for (unsigned i = 0; i < size; ++i)
  {
    if (sources_[i]->Name() == name && sources_[i]->hole_ == hole)
    {
      return sources_[i];
    }
  }
  SpriteSource* source = new SpriteSource(name, rows, cols, hole);
  sources_.push_back(source);

  return source;
}

Mesh * TextureSys::FindMesh(unsigned x, unsigned y, MeshType typeMesh)
{
  unsigned size = meshes_.size();

  for (unsigned i = 0; i < size; i++)
  {
    if (meshes_[i]->columns_ == x && meshes_[i]->rows_ == y && meshes_[i]->typeMesh_ == typeMesh)
    {
      //check if right
      return meshes_[i];
    }
  }

  Mesh* meshThing = new Mesh(x, y, typeMesh);

  meshes_.push_back(meshThing);

  return meshThing;
}
