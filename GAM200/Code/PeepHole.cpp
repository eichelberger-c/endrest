///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: PeepHole.cpp
//
//  Author: Charles Eichelberger
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -PeepHole
//      -~PeepHole
//      - Initialize
//      - Update
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "ObjManager.h"
#include "Transform.h"
#include "ObjFactory.h"
#include "Spaces.h"
#include "Animation.h"
#include "PeepHole.h"
#include "Trace.h"
#include "spawner.h"
#include "Engine.h"
#include "Camera.h"
#include "Graphics.h"
#include "Lighting.h"
///----------------------------------GLOBALS-----------------------------------

///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Construct PeepHole
PeepHole::PeepHole() : Obj("PeepHole")
{
}

///-------------------------------------
/// Brief: Initilize player
void PeepHole::Initialize()
{
  Transform* backgroundTransform = new Transform(0,0);
  backgroundTransform->Scale(1920, 1080);
  Timer* timer = new Timer(5, CountUp_, 5);
  Sprite* sprite = new Sprite(std::string("Data/Textures/default.png"), std::string("Data/Textures/fog_alpha_mask.png"), 
    glm::vec2(1,1), std::string("Shaders/holeVert.glsl"), std::string("Shaders/holeFrag.glsl"), true);
  sprite->SetRGBA(glm::vec4(0, 0, 0, .99f));
  Add(sprite);
  Add(backgroundTransform);
  Add(timer);
}

///-------------------------------------
/// Brief: update player
void PeepHole::Update(float dt)
{

  if (Has(Timer)->Beep())
  {
    ScaleUp();
    UpdateSize();
    Has(Timer)->Reset();
  }
  Vec2D playerLoc;
  Player* player = &Space::GetPlayer();
  playerLoc = player->Has(Transform)->Translation();
  glm::vec2 pCoords;
  pCoords.x = -playerLoc.X();
  pCoords.y = playerLoc.Y();
  alpha_coord = Graphics::WorldToTexture(pCoords, currSize_);

  Obj::Update(dt);
}

void PeepHole::ScaleUp()
{
  switch (holeSize_)
  {
    case minSize_:
    {
      holeSize_ = middleSize_;
      break;
    }
    case middleSize_:
    {
      holeSize_ = maxSize_;
      break;
    }
    case maxSize_:
    {
      holeSize_ = maxSize_;
      break;
    }
  }

}

void PeepHole::ScaleDown()
{
  switch (holeSize_)
  {
    case maxSize_:
    {
      holeSize_ = middleSize_;
      break;
    }
    case middleSize_:
    {
      holeSize_ = minSize_;
      break;
    }
    case minSize_:
    {
      holeSize_ = minSize_;
      break;
    }
  }
}

void PeepHole::UpdateSize()
{
  switch (holeSize_)
  {
    case maxSize_:
    {
      currSize_ = max_;

      break;
    }
    case minSize_:
    {
      currSize_ = min_;
      
      break;
    }
    case middleSize_:
    {
      currSize_ = middle_;
      break;
    }
  }
  reinterpret_cast<Lighting*>(Space::GetPlayer().Has(Lighting))->Scale(currSize_);
}

void PeepHole::Render()
{
  // If the object does not exist or is diabled dont draw
  if (!IsActive())
    return;

  Sprite *objSprite = Has(Sprite);
  Transform *trans = Has(Transform);

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
  /*
  std::vector<glm::vec2&> positions = 
  std::vector<float&> scales = 
  */
  // Send everything needed to the shader
  shader->SendInt("textId", 0);
  shader->SendMat3("transform", viewMat);
  shader->SendVec4("rgba", RGBA[0], RGBA[1], RGBA[2], RGBA[3]);
  shader->SendMat4("OrthoView", Camera::ProjectionMat());
  shader->SendFloat("size", 1 / currSize_);
  if (objSprite->GetMaskSpriteSource())
  {
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, objSprite->GetMaskSpriteSource()->GetId());

    shader->SendInt("maskId", 1);


    GLint uniformVarLoc = glGetUniformLocation(shader->shaderProgram, "texCoord");
    glUniform2fv(uniformVarLoc, 1, &alpha_coord[0]);
  }

  //Tell OpenGL to draw the object
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  shader->Unuse();

  // added by megan -- draws sub objects
  std::vector<Obj*>subList = GetSubObjList();
  for (Obj* obj : subList)
    obj->Render();
}