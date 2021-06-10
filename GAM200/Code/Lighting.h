///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Lighting.h
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Lighting
//      - Lighting (copy constructor)
//      - Update (virtual)
//      - Clone (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "stdafx.h"
#include "Component.h"
#include "Sprite.h"
#include "Obj.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class Lighting : public Component
{
public:
  Lighting(std::string name);
  ~Lighting();
  Lighting(const Component& ogComponent);
  virtual Component * Clone() const;
  virtual void Update(float dt);

  inline glm::vec4 Color() { return light_->Has(Sprite)->GetRGBA(); }
  inline void Color(glm::vec4 color) { light_->Has(Sprite)->SetRGBA(color); bool t = true; light_->Has(Sprite)->SetVisible(t); }
  inline float& Scale() { return scale_; }
  inline void Scale(float scale) { scale_ = scale; isDirty_ = true; }
  inline void Position(glm::vec2 position) { position_ = position; }
  inline glm::vec2& Position() { return position_; }
  inline bool IsDirty() { return isDirty_; }
  inline void IsDirty(bool isDirty) { isDirty_ = isDirty; }
  inline bool Toggle() { return IsActive(); }
  inline void Toggle(bool tog) { IsActive(tog); light_->IsActive(tog); }

private:
  Obj * light_;
  float scale_ = 2.0f;
  glm::vec2 position_;
  bool isDirty_ = true;
  bool init_ = true;
  //some sweet private stuff I'm sure you'll remember to put trailing underscores on
};

///---------------------------------FUNCTIONS-----------------------------------


