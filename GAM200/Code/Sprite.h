///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Sprite.h
//
//  Author: Sonya Toft
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
#include "SpriteSource.h"
#include "Mesh.h"
#include "Shader.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class Sprite : public Component
{
public:
  Sprite(std::string name = "Data/Textures/default.png", std::string maskName = "", glm::uvec2 dimensions = glm::uvec2(1, 1),
    std::string vertShader = "Shaders/defaultVert.glsl", 
    std::string fragShader = "Shaders/defaultFrag.glsl", bool hole = false);


  Sprite(const Sprite& ogSprite);
  Sprite(Component* ogSprite);

  ~Sprite();

  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset() {} // write me



  glm::vec4 GetRGBA() const;
  void SetRGBA(glm::vec4 RGBAog);


  float GetAlpha() const;
  void SetAlpha(float newAlpha);
  void AdjustAlpha(float& value);

  std::string GetName() const;
  void SetIndex(unsigned int index);
  SpriteSource* GetSpriteSource();
  SpriteSource* GetMaskSpriteSource();
  unsigned int GetFrameIndex();
  void SetSpriteSource(SpriteSource* newSource);
  bool GetVisible();
  void SetVisible(bool& visibility);

  //?
  void SetMesh(Mesh *newMesh);
  Mesh* GetMesh() const;

  int GetImgWidth();
  int GetImgHeight();

  int GetMask();

  inline Shader* GetShader() { return shader_; };
  inline void SetShader(Shader* shader) { shader_ = shader; }

  void Flash(glm::vec3 color, glm::vec3 ogcolor, int length);

  inline bool isFlash() { return flash_; }

private:
  glm::vec4 RGBA_;
  std::string name_;
  std::string maskName_;
  int hasMask_ = 0;

  unsigned int frameIndex_ = 0;
  bool isVisible_;
  SpriteSource* image_;
  SpriteSource* maskImage_ = nullptr;
  glm::vec4 ogColor_;
  glm::vec4 flashColor_;
  Mesh * MeshHold_;
  unsigned char * ImageHold_;
  int imgWidth_;
  int imgHeight_;
  int colorLenght_;
  float colorTimer_;
  bool flash_ = false;
  Shader* shader_;
};

///---------------------------------FUNCTIONS-----------------------------------


