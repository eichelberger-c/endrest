///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Sprite.cpp
//
//  Author: Sonya Toft / Thornton Stockwin
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Spaces.h"
#include "Transform.h"
#include "Obj.h"
#include <SOIL2.h>  
#include "Engine.h"
#include "TextureSys.h"
#include "ShaderSys.h"

///----------------------------------GLOBALS-----------------------------------




///---------------------------------FUNCTIONS-----------------------------------

Sprite::Sprite(std::string name, std::string maskName, glm::uvec2 dimensions, std::string vertShader, std::string fragShader, bool hole) : Component(Component::Sprite_)
{
  TextureSys* textSys = reinterpret_cast<TextureSys*>(EngineGetSystem("TextureSys"));
  frameIndex_ = 0;
  RGBA_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  name_ = name;
  maskName_ = maskName;
  imgHeight_ = dimensions.y;
  imgWidth_ = dimensions.x;
  image_ = textSys->FindSource(name, dimensions.x, dimensions.y);
  isVisible_ = true;
  colorLenght_ = 0;

  if (!maskName.empty())
  {
    hasMask_ = 1;
    maskImage_ = textSys->FindSource(maskName, dimensions.x, dimensions.y, hole);
  }

  shader_ = reinterpret_cast<ShaderSys*>(EngineGetSystem("ShaderSys"))->FindShader(vertShader, fragShader);
}

Sprite::Sprite(const Sprite& ogSprite) : Component(Component::Sprite_)
{
  frameIndex_ = ogSprite.frameIndex_;
  RGBA_ = ogSprite.RGBA_;
  name_ = ogSprite.name_;
  maskName_ = ogSprite.maskName_;
  hasMask_ = ogSprite.hasMask_;
  colorLenght_ = 0;
  MeshHold_ = ogSprite.MeshHold_;

  ImageHold_ = ogSprite.ImageHold_;
  imgWidth_ = ogSprite.imgWidth_;
  imgHeight_ = ogSprite.imgHeight_;

  SpriteSource* ogImage = ogSprite.image_;
  image_ = reinterpret_cast<TextureSys*>(EngineGetSystem("TextureSys"))->FindSource(ogImage->Name(), unsigned(ogImage->GetDimensions().x), unsigned(ogImage->GetDimensions().y));

  isVisible_ = ogSprite.isVisible_;
  Parent(ogSprite.Parent());

  shader_ = ogSprite.shader_;

}
Sprite::Sprite(Component* sprite) : Component(Sprite_)
{
  if (sprite)
  {
    Sprite* ogSprite = dynamic_cast<Sprite*>(sprite);
    frameIndex_ = ogSprite->frameIndex_;
    RGBA_ = ogSprite->RGBA_;
    name_ = ogSprite->name_;
    maskName_ = ogSprite->maskName_;
    hasMask_ = ogSprite->hasMask_;
    colorLenght_ = 0;
    MeshHold_ = ogSprite->MeshHold_;

    ImageHold_ = ogSprite->ImageHold_;
    imgWidth_ = ogSprite->imgWidth_;
    imgHeight_ = ogSprite->imgHeight_;
    SpriteSource* ogImage = ogSprite->image_;
    image_ = reinterpret_cast<TextureSys*>(EngineGetSystem("TextureSys"))->FindSource(ogImage->Name(), unsigned(ogImage->GetDimensions().x), unsigned(ogImage->GetDimensions().y));
    isVisible_ = ogSprite->isVisible_;
    Parent(ogSprite->Parent());

    shader_ = ogSprite->shader_;
  }
}

Sprite::~Sprite()
{
}

Component * Sprite::Clone() const
{
  Sprite *sprite = new Sprite(name_, maskName_);
  if (sprite == NULL)
    return NULL;

  *sprite = *this;

  return sprite;
}

void Sprite::Update(float dt)
{
  if (!IsActive())
    return;

  if (flash_)
  {
    colorTimer_ += 1;
    if (colorTimer_ < 45)
    {
      if (int(colorTimer_) % 15 != 0)
      {
        SetRGBA(flashColor_);
      }
      else
      {
        SetRGBA(ogColor_);
      }
    }
    else
    {
      colorTimer_ = 0;
      flash_ = false;
      SetRGBA(ogColor_);
    }

  }

  if (RGBA_[3] == 0)
  {
    bool b = false;
    SetVisible(b);
  }
  image_->Update();
  if(maskImage_)
    maskImage_->Update();
}

///-------------------------------------
/// Brief: Get alpha value
glm::vec4 Sprite::GetRGBA() const
{
  return RGBA_;
}

void Sprite::SetRGBA(glm::vec4 RGBAog)
{
  RGBA_ = RGBAog;
}

///-------------------------------------
/// Brief: Get alpha value
float Sprite::GetAlpha() const
{
  return RGBA_[3];
}

void Sprite::SetAlpha(float newAlpha)
{
  RGBA_[3] = newAlpha;
}

void Sprite::AdjustAlpha(float& value)
{
  RGBA_[3] += value;

  if (RGBA_[3] > 1.0)
    RGBA_[3] = 1.0f;
  if (RGBA_[3] < 0.0)
    RGBA_[3] = 0.0f;
}

void Sprite::SetMesh(Mesh *newMesh)
{
  MeshHold_ = newMesh;
}

Mesh* Sprite::GetMesh() const
{
  return MeshHold_;
}
int Sprite::GetImgWidth()
{
  return imgWidth_;
}
int Sprite::GetImgHeight()
{
  return imgHeight_;
}
///-------------------------------------
/// Brief: Get name
std::string Sprite::GetName() const
{
  return name_;
}

SpriteSource* Sprite::GetSpriteSource()
{
  return image_;
}

void Sprite::SetSpriteSource(SpriteSource* newSource)
{
  image_ = newSource;
}

SpriteSource * Sprite::GetMaskSpriteSource()
{
  return maskImage_;
}

unsigned int Sprite::GetFrameIndex()
{
  return frameIndex_;
}

void Sprite::SetIndex(unsigned int index)
{
  frameIndex_ = index;
}

bool Sprite::GetVisible()
{
  return isVisible_;
}

void Sprite::SetVisible(bool& visibility)
{
  isVisible_ = visibility;
}

int Sprite::GetMask()
{
  return hasMask_;
}

void Sprite::Flash(glm::vec3 color, glm::vec3 ogColor, int length)
{
  ogColor_ = glm::vec4(ogColor, 1);
  flashColor_ = glm::vec4(color, 1);
  colorLenght_ = length;
  flash_ = true;
}