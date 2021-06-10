///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Transform.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Transform
//      - Transform (copy constructor)
//      - Update (virtual)
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once

#include "Vec2D.h"
#include "Matrix2D.h"
#include "Component.h"
#include "glm\gtc\matrix_transform.hpp"
//#include "Mat2D.h"

///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------

class Transform : public Component
{
public:
  Transform(float x, float y);
  Transform(Vec2D translation);
  Transform(const Transform& ogTransform);
  Transform(Component* transform);
  ~Transform();
  virtual Component * Clone() const;
  virtual void Update(float dt);
  virtual void Reset(); // written but check if anything else needed
  float Rotation();
  Vec2D Scale();
  void Scale(float x, float y);
  void MultScale(float scale);
  float MultScale();
  float ScaleSquare();
  void Rotation(float rotation);
  void inline Translation(float x, float y) { translation_.X(x); translation_.Y(y); }
  void Translation(glm::vec2 newTranslation);
  void Translation(Vec2D newTranslation);
  Vec2D Translation() { return translation_; }
  glm::vec2 glmTranslation();
  glm::vec2 glmScale();
  glm::mat3* TransMatrix();
  void setDirty(bool);
  //void Matrix(Matrix2D newTranslation);

  void OGTrans(Vec2D translation);
  void OGTrans(glm::vec2 translation);
  Vec2D OGTrans() { return ogTranslation_;}

  friend class Physics;

private:
  float rotation_;
  Vec2D scale_;
  Vec2D translation_;
  glm::mat3 matrix_;
  bool isDirty_;

  float multscale_ = 1.0f;
  Vec2D ogScale_;

  Vec2D ogTranslation_; // for serialization/editor
};

///---------------------------------FUNCTIONS-----------------------------------


