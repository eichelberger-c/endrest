///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Transform.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief:
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Transform.h"
#include "Spaces.h"
#include "Lighting.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

Transform::Transform(float x, float y) : Component(Component::Transform_)
{
  rotation_ = 0.0f;
  translation_.Set(x, y);
  scale_.Set(100.0f, 100.0f);
  ogScale_ = scale_;

  //Set scale in major diagonal
  matrix_[0][0] = scale_.X();
  matrix_[1][1] = scale_.Y();

  //Set translation in last column
  matrix_[2][0] = translation_.X();
  matrix_[2][1] = translation_.Y();

  //Homogenous coordinate
  matrix_[2][2] = 1.0f;

  isDirty_ = true;

  //Space::AddTransform(this);
}

Transform::Transform(Vec2D translation) : Component(Component::Transform_)
{
  rotation_ = 0.0f;
  translation_.Set(translation.x_, translation.y_);
  scale_.Set(100.0f, 100.0f);
  ogScale_ = scale_;

  //Set scale in major diagonal
  matrix_[0][0] = scale_.X();
  matrix_[1][1] = scale_.Y();

  //Set translation in last column
  matrix_[2][0] = translation_.X();
  matrix_[2][1] = translation_.Y();

  //Homogenous coordinate
  matrix_[2][2] = 1.0f;
}

Transform::Transform(const Transform& ogTransform) : Component(Component::Transform_)
{
  rotation_ = ogTransform.rotation_;
  translation_.Set(ogTransform.translation_.x_, ogTransform.translation_.y_);
  scale_ = ogTransform.scale_;
  ogScale_ = scale_;
  multscale_ = ogTransform.multscale_;
  IsActive(ogTransform.IsActive());
  Parent(ogTransform.Parent());
  //Space::AddTransform(this);
}

Transform::Transform(Component* transform) : Component(Transform_)
{
  if (transform)
  {
    Transform* ogTransform = dynamic_cast<Transform*>(transform);
    rotation_ = ogTransform->rotation_;
    translation_.Set(ogTransform->translation_.x_, ogTransform->translation_.y_);
    scale_ = ogTransform->scale_;
    ogScale_ = scale_;
    multscale_ = ogTransform->multscale_;
    IsActive(ogTransform->IsActive());
    Parent(ogTransform->Parent());
  }
}

Transform::~Transform()
{

}

Component * Transform::Clone() const
{
  Transform *trans = new Transform(translation_.x_, translation_.y_);
  if (trans == NULL)
    return NULL;

  *trans = *this;

  return trans;
}

///-------------------------------------
/// Brief: Concatenates a new matrix if the matrix needs to change
void Transform::Update(float dt)
{
  if (!IsActive())
    return;

  if (isDirty_)
  {
    //Temporary matrices
    Matrix2D scaleM;
    Matrix2D rotateM;
    Matrix2D translateM;
    glm::mat3 tempMatrix(0.0f);

    Vec2D scale = Scale();
    Vec2D translation = translation_;

    scaleM.Matrix2DScale(scale.x_, scale.y_);

    if (Parent()->Name().compare("Lantern") == 0 || Parent()->Name().compare("LanternVector") == 0 || Parent()->Name().compare("LanternVector2") == 0)
      rotateM.Matrix2DRotRadLight(rotation_);
    else
      rotateM.Matrix2DRotRad(rotation_);

    translateM.Matrix2DTranslate(translation.x_, translation.y_);

    translateM *= rotateM;
    translateM *= scaleM;



    for (unsigned int i = 0; i < 3; ++i)
    {
      for (unsigned int j = 0; j < 3; ++j)
      {
        tempMatrix[i][j] = translateM.Index(j, i);
      }
    }

    isDirty_ = false;
    matrix_ = tempMatrix;
  }
}

void Transform::Reset()
{
  translation_ = ogTranslation_;
  isDirty_ = true;
}

///-------------------------------------
/// Brief: Get rotation
float Transform::Rotation()
{
  return rotation_;
}

///-------------------------------------
/// Brief: Get scale
Vec2D Transform::Scale()
{
  return scale_;
}

glm::vec2 Transform::glmScale()
{
  return glm::vec2(scale_.X(), scale_.Y());
}

void Transform::Scale(float x, float y)
{
  scale_.x_ = x;
  scale_.y_ = y;
  isDirty_ = true;
}

void Transform::MultScale(float scale)
{
  multscale_ = scale;
  scale_.x_ = ogScale_.X() * scale;
  scale_.y_ = ogScale_.Y() * scale;
  isDirty_ = true;
}

float Transform::MultScale()
{
  return multscale_;
}

float Transform::ScaleSquare()
{
  if (scale_.x_ == scale_.y_)
    return scale_.x_;

  return 0.0f;
}

glm::mat3* Transform::TransMatrix()
{
  return &matrix_;
}

void Transform::Rotation(float rotation)
{
  rotation_ = rotation;
}

void Transform::Translation(Vec2D newTranslation)
{
  translation_ = newTranslation;
  isDirty_ = true;
  Lighting* light = Parent()->Has(Lighting); 
  if (light)
  {
    light->IsDirty(true);
  }
}

void Transform::Translation(glm::vec2 newTranslation)
{
  float x = newTranslation.x;
  float y = newTranslation.y;
  translation_.Set(x, y);
  isDirty_ = true;
  Lighting* light = Parent()->Has(Lighting);
  if (light)
  {
    light->IsDirty(true);
  }
}

glm::vec2 Transform::glmTranslation()
{
  float x = translation_.X();
  float y = translation_.Y();
  glm::vec2 returnValue(x, y);
  return returnValue;
}
void Transform::setDirty(bool dirty)
{
  isDirty_ = dirty;
}

void Transform::OGTrans(Vec2D translation)
{
  ogTranslation_ = translation;
}

void Transform::OGTrans(glm::vec2 translation)
{
  float x = translation.x;
  float y = translation.y;
  ogTranslation_.Set(x, y);
}
