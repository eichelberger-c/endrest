///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: SpriteSource.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "SpriteSource.h"
#include <SOIL2.h> 
#include "Engine.h"
#include "TextureSys.h"
///----------------------------------GLOBALS-----------------------------------
static std::vector<SpriteSource*> sources;
///---------------------------------FUNCTIONS-----------------------------------
SpriteSource::SpriteSource(const std::string &name, unsigned int rows, unsigned int cols, bool hole)
{
  name_ = name;
  glGenTextures(1, &textID_);
  glBindTexture(GL_TEXTURE_2D, textID_);
  rows_ = rows;
  cols_ = cols;
  hole_ = hole;

  //repeat image if it doesn't fill mesh
  if (hole)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  }
  else
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  }

  //Adjust image for if there is zoom in and out
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  ImageHold_ = SOIL_load_image(name.c_str(), &imgWidth_, &imgHeight_, NULL, SOIL_LOAD_RGBA);

  if (ImageHold_)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth_ , imgHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageHold_);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    TraceMessage(GraphicsLog, "ERROR::TEXTURE_LOADING_FAILED");
  }

  MeshHold_ = reinterpret_cast<TextureSys*>(EngineGetSystem("TextureSys"))->FindMesh(rows, cols, MeshType::Square_);

}

SpriteSource::SpriteSource(const SpriteSource& ogSpriteSource)
{
  name_ = ogSpriteSource.name_;
  imgWidth_ = ogSpriteSource.imgWidth_;
  imgHeight_ = ogSpriteSource.imgHeight_;
  ImageHold_ = ogSpriteSource.ImageHold_;
  MaskHold_ = ogSpriteSource.MaskHold_;
  textID_ = ogSpriteSource.textID_;
  MeshHold_ = ogSpriteSource.MeshHold_;
  rows_ = ogSpriteSource.rows_;
  cols_ = ogSpriteSource.cols_;
}

SpriteSource::~SpriteSource()
{
    if(ImageHold_)
      SOIL_free_image_data(ImageHold_);
}

void SpriteSource::Update()
{
  if (isDirty_)
  {
    if(ImageHold_)
      SOIL_free_image_data(ImageHold_);
    ImageHold_ = SOIL_load_image(name_.c_str(), &imgWidth_, &imgHeight_, NULL, SOIL_LOAD_RGBA);

    if (ImageHold_)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth_, imgHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageHold_);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    isDirty_ = false;
  }
}
unsigned int SpriteSource::GetId()
{
  return textID_;
}

unsigned int SpriteSource::GetMaskId()
{
  return maskID_;
}

Mesh& SpriteSource::GetMesh()
{
  return *MeshHold_;
}

void SpriteSource::SetMesh(Mesh& mesh)
{
  *MeshHold_ = mesh;
}

glm::vec2 SpriteSource::GetUV(unsigned int frameIndex)
{
  glm::vec2 uv(0, 0);
  glm::vec2 rowscols = MeshHold_->GetNumRowsCols();
  int x = int(rowscols.x);
  int y = int(rowscols.y);
  float uSize = 1.0f / x;
  float vSize = 1.0f / y;

  uv.x = (vSize * (frameIndex % x));
  uv.y = uSize + (frameIndex / x) * uSize;
  return uv;
}

std::string& SpriteSource::Name()
{
  return name_;
}

void SpriteSource::Name(std::string name)
{
  name_ = name;
  isDirty_ = true;
}