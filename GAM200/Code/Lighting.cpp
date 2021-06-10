///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Lighting.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: Does some graphic stuff 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

#include "Lighting.h"
#include "Transform.h"
#include "Graphics.h"
#include "Sprite.h"
Lighting::Lighting(std::string name) : Component(Lighting_)
{
  light_ = new Obj(name);
  Transform* trans = new Transform(0, 0);
  trans->Scale(1920, 1080);
  light_->Add(trans);
  Sprite* sprite = new Sprite("Data/Textures/fog_alpha_mask.png");
  sprite->SetRGBA(glm::vec4(.898f, .898f, .4f, 0.f)); 
  light_->Add(sprite);
}


Lighting::~Lighting()
{
  //delete light_;
}

Lighting::Lighting(const Component & ogComponent) : Component(Lighting_)
{
  const Lighting& ogLight = dynamic_cast<const Lighting&>(ogComponent);
  light_ = new Obj(*(ogLight.light_));
  scale_ = ogLight.scale_;
  position_ = ogLight.position_;
}

Component * Lighting::Clone() const
{
  return new Component(*this);
}

void Lighting::Update(float dt)
{
  if (init_)
  {
    Parent()->AddSubObj(light_);
    init_ = false;
  }

  Transform* trans = light_->Has(Transform);
  Transform* parentTrans = Parent()->Has(Transform);
  trans->Translation(parentTrans->Translation());

  if (isDirty_)
  {
    Vec2D scale = trans->Scale();
    glm::vec2 translation = trans->glmTranslation();
    position_ = Graphics::WorldToTexture(translation, scale_);
    trans->Scale(1920 * scale_, 1080 * scale_);
    isDirty_ = false;
  }

  light_->Update(dt);  
}
