/////-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: HealthBar.cpp
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------
#define _USE_MATH_DEFINES
#include <cmath>
///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "HealthBar.h"
#include "Camera.h"


///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Initializes player's health bar
void HealthBar::Initialize()
{
  player_ = &Space::GetPlayer();

  // BASE OBJECT (THIS)
  Transform *transform = new Transform(-850.0f, 450.0f);
  transform->MultScale(1.5f);
  Add(transform);
  Add(new Sprite("Data/Textures/UI_Healthbar_Background.png", ""));


  // subobjects
  base_ = new Obj("HBbase");
  glow_ = new Obj("HBglow");


  // BASE
  Sprite* baseSprite = new Sprite("Data/Textures/UIHealthbarBaseColor001.png", "Data/Textures/UIHealthbarBaseColorAlphaMask001.png",
  glm::vec2(1, 1), std::string("Shaders/healthBarVert.glsl"), std::string("Shaders/healthBarFrag.glsl"));
  base_->Add(baseSprite);
  Transform* baseTrans = new Transform(-850.0f, 450.0f);
  baseTrans->MultScale(1.5f);
  base_->Add(baseTrans);

  // GLOW
  Sprite* glowSprite = new Sprite("Data/Textures/UIHealthbarGlowColor001.png", "Data/Textures/UIHealthbarGlowColorAlphaMask001.png",
  glm::vec2(1, 1), std::string("Shaders/healthBarVert.glsl"), std::string("Shaders/healthBarFrag.glsl"));
  glow_->Add(glowSprite);
  Transform* glowTrans = new Transform(-850.0f, 450.0f);
  glowTrans->MultScale(1.5f);
  glow_->Add(glowTrans);

  baseTrans->setDirty(true);
  glowTrans->setDirty(true);
  baseTrans->Update(0.0f);
  glowTrans->Update(0.0f);


  // LANTERN & ORB
  lantern_ = new Obj("HBlantern");
  lantern_->Add(new Sprite("Data/Textures/UI_Healthbar_Lantern.png", ""));
  lantern_->Add(new Transform(transform));

  orb_ = new Obj("HBorb");
  orb_->Add(new Sprite("Data/Textures/UI_Healthbar_Orb.png", ""));
  orb_->Add(new Transform(transform));

  Transform *ltrans = lantern_->Has(Transform);
  Transform *otrans = orb_->Has(Transform);

  ltrans->setDirty(true);
  otrans->setDirty(true);
  ltrans->Update(0.0f);
  otrans->Update(0.0f);


  // KEYS
  key_ = new Obj(*Space::GetArchetype("KeySprite"), "HBkey");
  bluekey_ = new Obj(*Space::GetArchetype("KeySprite"), "HBredkey");
  redkey_ = new Obj(*Space::GetArchetype("KeySprite"), "HBbluekey");
  purplekey_ = new Obj(*Space::GetArchetype("KeySprite"), "HBpurplekey");

  bool start = false;
  key_->IsActive(start);
  bluekey_->IsActive(start);
  redkey_->IsActive(start);
  purplekey_->IsActive(start);

  // set colors
  bluekey_->Has(Sprite)->SetRGBA(glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
  redkey_->Has(Sprite)->SetRGBA(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  purplekey_->Has(Sprite)->SetRGBA(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  
  
  // inits transforms
  trans1_ = key_->Has(Transform);
  trans2_ = bluekey_->Has(Transform);
  trans4_ = redkey_->Has(Transform);
  trans3_ = purplekey_->Has(Transform);

  // stuff i guess
  trans1_->Translation(vec1_);
  trans1_->Rotation(0.90f);
  trans1_->setDirty(true);
  trans1_->Update(0.0f);
  trans2_->Translation(vec2_);
  trans2_->Rotation(0.90f);
  trans2_->setDirty(true);
  trans2_->Update(0.0f);
  trans4_->Translation(vec4_);
  trans4_->Rotation(0.90f);
  trans4_->setDirty(true);
  trans4_->Update(0.0f);
  trans3_->Translation(vec3_);
  trans3_->Rotation(0.90f);
  trans3_->setDirty(true);
  trans3_->Update(0.0f);

  // adds to subobject lists
  AddSubObj(lantern_);
  AddSubObj(orb_);
  AddSubObj(key_);
  AddSubObj(bluekey_);
  AddSubObj(redkey_);
  AddSubObj(purplekey_);

  // starts as inactive
  bool bill = false;
  IsActive(bill);
}

///-------------------------------------
/// Brief: updates health bar as player's health changes
void HealthBar::Update(float dt)
{
  Obj::Update(dt);
}

///-------------------------------------
/// Brief: renders healthbar and subobjects
void HealthBar::Render()
{
  if (!IsActive())
    return;

  alphaRender(base_);
  alphaRender(glow_);

  std::vector<Obj*>subobjs = Obj::GetSubObjList();
  int size = subobjs.size();

  for (int i = 0; i < size; ++i)
    subobjs[i]->Render();
}

///-------------------------------------
/// Brief: renders subobjects with alpha masks
void HealthBar::alphaRender(Obj* obj)
{

  // If the object does not exist or is diabled dont draw
  if (!IsActive())
    return;

  Sprite *objSprite = obj->Has(Sprite);
  Transform *trans = obj->Has(Transform);

  // If the object does not have the right components do not draw
  if (objSprite == nullptr || !objSprite->GetVisible() || trans == nullptr)
    return;

  Shader* shader = objSprite->GetShader();

  if (!shader)
    return;

  // Get the mesh ids and sprite varaibles needed to send to shader
  SpriteSource* spriteSource = objSprite->GetSpriteSource();
  Mesh * objMesh = &spriteSource->GetMesh();
  GLuint VAO = objMesh->MeshGetID();
  glm::vec2 uv = spriteSource->GetUV(objSprite->GetFrameIndex());
  glm::vec4 RGBA = objSprite->GetRGBA();
  glm::mat3 *m = trans->TransMatrix();

  //Bind the opengl vao and textures to draw
  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, spriteSource->GetId());

  shader->UseShader();

  glm::mat3 viewMat = Camera::ViewMat() * *m;

  // Send everything needed to the shader
  shader->SendInt("textId", 0);
  shader->SendVec2("uv", uv[0], uv[1]);
  shader->SendMat3("transform", viewMat);
  shader->SendVec4("rgba", RGBA[0], RGBA[1], RGBA[2], RGBA[3]);
  shader->SendMat4("OrthoView", Camera::ProjectionMat());

  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, objSprite->GetMaskSpriteSource()->GetId());

  shader->SendInt("maskId", 1);

  float currentHealth = float(Space::GetPlayer().Has(Health)->HitPoints());
  float maxHealth = float(Space::GetPlayer().Has(Health)->MaxHP());

  float percentHealth = currentHealth / maxHealth;
  shader->SendFloat("hpPercent", percentHealth);

  //Tell OpenGL to draw the object
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  shader->Unuse();
}


///-------------------------------------
/// Brief: sets HB key alphas
void HealthBar::SetKeyActive(colorFlag color, bool yup)
{
  bool active = yup;

  if (color == NoColor_)
    key_->IsActive(active);
  else if (color == Blue_)
    bluekey_->IsActive(active);
  else if (color == Purple_)
    purplekey_->IsActive(active);
  else if (color == Red_)
    redkey_->IsActive(active);
}
