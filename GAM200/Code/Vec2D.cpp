///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Vec2D.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Vec2D (constructor)
//      - Vec2D (copy constructor)
//      - Vec2D (construct from angle)
//      - Set
//      - GetLength
//      - operator=
//      - operator*=
//      - operator%
//      - operator~
//      - operator^
//      - operator+
//      - operator-
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Vec2D.h"
#include "cmath"
#include "Mat2D.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Constructor
Vec2D::Vec2D()
{
  x_ = 0.0f;
  y_ = 0.0f;
}

///-------------------------------------
/// Brief: Copy constructor
Vec2D::Vec2D(const Vec2D& og)
{
  x_ = og.x_;
  y_ = og.y_;
}

///-------------------------------------
/// Brief: Conversion constructor (from radians)
Vec2D::Vec2D(float angle)
{
  x_ = std::cos(angle);
  y_ = std::sin(angle);
}

Vec2D::Vec2D(float x, float y)
{
  x_ = x;
  y_ = y;
}

///-------------------------------------
/// Brief: Set the vector values with floats
void Vec2D::Set(float x, float y)
{
  x_ = x;
  y_ = y;
}

///-------------------------------------
/// Brief: operator=
Vec2D& Vec2D::operator=(const Vec2D& rhv)
{
  x_ = rhv.x_;
  y_ = rhv.y_;

  return *this;
}

///-------------------------------------
/// Brief: operator*
Vec2D Vec2D::operator*(const Vec2D& rhv) const
{
  Vec2D vec(*this);

  vec.x_ = vec.x_ * rhv.x_;
  vec.y_ = vec.y_ * rhv.y_;

  return vec;
}

Vec2D Vec2D::operator*(float scalar) const
{
  Vec2D temp(*this);

  temp.x_ *= scalar;
  temp.y_ *= scalar;

  return temp;
}

///-------------------------------------
/// Brief: operator*=
Vec2D& Vec2D::operator*=(int scalar)
{
  x_ = x_ * scalar;
  y_ = y_ * scalar;

  return *this;
}

Vec2D & Vec2D::operator*=(const Mat2D & mat)
{
  Vec2D temp(*this);

  x_ = (mat.m_[0][0] * temp.x_) + (mat.m_[0][1] * temp.y_) + (mat.m_[0][2]);
  y_ = (mat.m_[1][0] * temp.x_) + (mat.m_[1][1] * temp.y_) + (mat.m_[1][2]);

  return *this;
}

///-------------------------------------
/// Brief: Scales Vector by float
Vec2D& Vec2D::operator*=(float scalar)
{

  x_ *= scalar;
  y_ *= scalar;

  return *this;
}

///-------------------------------------
/// Brief: Adds float to this vector
Vec2D Vec2D::operator+=(float rhv)
{
  x_ += rhv;
  y_ += rhv;

  return *this;
}

///-------------------------------------
/// Brief: Adds one vector to this
Vec2D Vec2D::operator+=(const Vec2D& rhv)
{
  x_ += rhv.x_;
  y_ += rhv.y_;

  return *this;
}

///-------------------------------------
/// Brief: Returns Dot Product of two vectors
float Vec2D::operator%(const Vec2D& rhv) const
{
  return (x_ * rhv.x_) + (y_ * rhv.y_);
}

///-------------------------------------
/// Brief: Returns the vector's length
float Vec2D::GetLength()
{
  float sum = (x_ * x_) + (y_ * y_);

  return std::sqrt(sum);
}

///-------------------------------------
/// Brief: operator+
Vec2D Vec2D::operator+(const Vec2D& rhv) const
{
  Vec2D vec(*this);
  vec.x_ = vec.x_ + rhv.x_;
  vec.y_ = vec.y_ + rhv.y_;

  return vec;
}

///-------------------------------------
/// Brief: operator- (binary)
Vec2D Vec2D::operator-(const Vec2D & rhv) const
{
  Vec2D vec(*this);
  vec.x_ = vec.x_ - rhv.x_;
  vec.y_ = vec.y_ - rhv.y_;

  return vec;
}

Vec2D Vec2D::operator+(float val) const
{
  Vec2D vec(*this);
  vec.x_ = vec.x_ - val;
  vec.y_ = vec.y_ - val;

  return vec;
}

Vec2D Vec2D::operator-(float val) const
{
  Vec2D vec(*this);
  vec.x_ = vec.x_ + val;
  vec.y_ = vec.y_ + val;

  return vec;
}

bool Vec2D::operator<(const Vec2D & rhv) const
{
  if (x_ < rhv.x_ || y_ < rhv.y_)
    return true;

  return false;
}

bool Vec2D::operator>(const Vec2D & rhv) const
{
  if (x_ > rhv.x_ || y_ > rhv.y_)
    return true;

  return false;
}

bool Vec2D::operator<=(const Vec2D & rhv) const
{
  if (x_ <= rhv.x_ || y_ <= rhv.y_)
    return true;

  return false;
}

bool Vec2D::operator>=(const Vec2D & rhv) const
{
  if (x_ >= rhv.x_ || y_ >= rhv.y_)
    return true;

  return false;
}

bool Vec2D::operator<=(float scalar) const
{
  if (x_ <= scalar && y_ <= scalar)
    return true;

  return false;
}

bool Vec2D::operator>=(float scalar) const
{
  if (x_ >= scalar && y_ >= scalar)
    return true;

  return false;
}

bool Vec2D::operator<(float scalar) const
{
  if (x_ < scalar && y_ < scalar)
    return true;

  return false;
}

bool Vec2D::operator>(float scalar) const
{
  if (x_ > scalar && y_ > scalar)
    return true;

  return false;
}

bool Vec2D::operator==(Vec2D & rhv) const
{
  if (x_ == rhv.x_ && y_ == rhv.y_)
    return true;
  else
    return false;
}

bool Vec2D::operator==(float scalar) const
{
  if (x_ == scalar && y_ == scalar)
    return true;
  else
    return false;
}

Vec2D Vec2D::operator-(float scalar)
{
  Vec2D temp(*this);

  temp.x_ -= scalar;
  temp.y_ -= scalar;

  return temp;
}

Vec2D& Vec2D::operator-()
{
  float x = -x_;
  float y = -y_;
  x_ = x;
  y_ = y;

  return *this;
}

Vec2D Vec2D::Abs() const
{
  Vec2D temp(*this);

  if (temp.x_ < 0)
    temp.x_ *= -1;
  if (temp.y_ < 0)
    temp.y_ *= -1;

  return temp;
}

void Vec2D::Zero()
{
  x_ = 0.0f;
  y_ = 0.0f;
}

///-------------------------------------
/// Brief: Normalizes the vector (unary)
Vec2D& Vec2D::operator~()
{
  float mag = GetLength();

  x_ = x_ / mag;
  y_ = y_ / mag;

  return *this;
}

///-------------------------------------
/// Brief: Returns the distance between vectors
float Vec2D::operator^(const Vec2D& rhv) const
{
  float sumx = rhv.x_ - x_;
  float sumy = rhv.y_ - y_;
  float total = (sumx * sumx) + (sumy * sumy);

  return std::sqrt(total);
}

double Vec2D::Distance(const Vec2D & rhv)
{
  double sumx = rhv.x_ - x_;
  double sumy = rhv.y_ - y_;
  double total = (sumx * sumx) + (sumy * sumy);

  return std::sqrt(total);
}

void Vec2D::Normalize()
{
  float length = GetLength();
  if (length > 0)
  {
    x_ /= length;
    y_ /= length;
  }
}

///-------------------------------------
/// Brief: Returns private X value
float Vec2D::X()
{
  return x_;
}

///-------------------------------------
/// Brief: Returns private Y value
float Vec2D::Y()
{
  return y_;
}

bool Vec2D::IsInRange(const Vec2D & vec, float range)
{
  float sumx = vec.x_ - x_;
  float sumy = vec.y_ - y_;
  float total = (sumx * sumx) + (sumy * sumy);

  float squared = range * range;

  if (total > squared)
    return false;

  return true;
}


