///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SpriteSource.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Sprite
//      - Sprite (copy constructor)
//      - Update (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

#define ASSET_PATH "./Images/"
///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Component.h"
#include "Mesh.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class SpriteSource
{
public:
  ~SpriteSource();
  void Update();
  virtual void Reset() {} // write me
  unsigned int GetId();
  unsigned int GetMaskId();

  void SetMesh(Mesh& mesh);
  std::string& Name();
  void Name(std::string name);

  inline glm::vec2 GetDimensions() { return glm::vec2(rows_, cols_); }

  Mesh& GetMesh();

  glm::vec2 GetUV(unsigned int framInex);

  _inline bool operator==(const std::string name) { return name_ == name; }

private:
  SpriteSource(const std::string &name, unsigned int rows = 1, unsigned int cols = 1, bool hole = false);
  SpriteSource(const SpriteSource& ogSpriteSource);
  std::string name_;
  bool hole_;
  int imgWidth_;
  int imgHeight_;
  int rows_;
  int cols_;
  int maskWidth_;
  int maskHeight_;

  unsigned char * ImageHold_;
  unsigned char * MaskHold_;

  unsigned int textID_;
  unsigned int maskID_;

  Mesh * MeshHold_;

  bool isDirty_ = false;

  friend class TextureSys;
};

///---------------------------------FUNCTIONS-----------------------------------


