///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Mat2D.cpp
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      -
//      -
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#define _USE_MATH_DEFINES

#include "stdafx.h"
#include "Mat2D.h"
#include "Vec2D.h"
#include <cmath>

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

Mat2D::Mat2D()
{
  for(int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      if (i == j)
        m_[i][j] = 1.0f;
      else
        m_[i][j] = 0.0f;
    }
}

Mat2D::Mat2D(const Mat2D & ogMat)
{
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      m_[i][j] = ogMat.m_[i][j];
}

Mat2D::Mat2D(float angle)
{
  ~(*this);

  m_[0][0] = (float)cos(angle);
  m_[0][1] = (float)-sin(angle);

  m_[1][0] = (float)sin(angle);
  m_[1][1] = (float)cos(angle);
}

Mat2D::Mat2D(float angle, Vec2D offset)
{
  ~(*this);

  float x = offset.x_;
  float y = offset.y_;
  float coz = (float)cos(angle);
  float zin = (float)sin(angle);

  m_[0][0] = coz;
  m_[0][1] = -zin;
  m_[0][2] = (float)(-x * coz + x + y * zin);

  m_[1][0] = zin;
  m_[1][1] = coz;
  m_[1][2] = (float)(-x * zin - y * coz + y);
}

Mat2D& Mat2D::LightRotation(float angle)
{
  float x = -32.0f;
  float y = 0.0f;
  float coz = (float)cos(angle);
  float zin = (float)sin(angle);

  m_[0][0] = coz;
  m_[0][1] = -zin;
  if ((angle <= M_PI_2 && angle >= 0) || (angle <= 0 && angle >= -M_PI_2))
    m_[0][2] = (float)(-x * coz + x + y * zin);
  if ((angle <= M_PI && angle >= M_PI_2) || (angle >= -M_PI && angle <= -M_PI_2))
    m_[0][2] = (float)(-x * coz + -x + y * zin);
  m_[1][0] = zin;
  m_[1][1] = coz;
  m_[1][2] = (float)(-x * zin - y * coz + y);

  return *this;
}

Mat2D::Mat2D(Vec2D translation)
{
  ~(*this);

  m_[0][2] = translation.x_;
  m_[1][2] = translation.y_;
}

Mat2D::Mat2D(Vec2D& scale)
{
  ~(*this);

  m_[0][0] = scale.x_;
  m_[1][1] = scale.y_;
}

Mat2D & Mat2D::operator~()
{
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      if (i == j)
        m_[i][j] = 1.0f;
      else
        m_[i][j] = 0.0f;
    }

  return *this;
}

bool Mat2D::operator==(const Mat2D & rhm)
{
  if (m_ != rhm.m_)
    return false;

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      if (m_[i][j] != rhm.m_[i][j])
        return false;

  return true;
}

bool Mat2D::operator!=(const Mat2D & rhm)
{
  if (m_ != rhm.m_)
    return true;

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      if (m_[i][j] != rhm.m_[i][j])
        return true;

  return false;
}

Mat2D Mat2D::operator+(const Mat2D & rhm)
{
  Mat2D temp(*this);

  for(int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      float val = rhm.m_[i][j];
      temp.m_[i][j] += val;
    }

  return temp;
}

Mat2D Mat2D::operator-(const Mat2D & rhm)
{
  Mat2D temp(*this);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      float val = rhm.m_[i][j];
      temp.m_[i][j] -= val;
    }

  return temp;
}

Mat2D Mat2D::operator*(const Mat2D & rhm)
{
  Mat2D temp(*this);
  Mat2D temp0(*this);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 3; ++k)
        temp.m_[i][j] += temp0.m_[i][k] * rhm.m_[k][j];

  return temp;
}

Mat2D Mat2D::operator*(float scalar)
{
  Mat2D temp(*this);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      temp.m_[i][j] *= scalar;

  return temp;
}

Mat2D & Mat2D::operator*=(const Mat2D & rhm)
{
  Mat2D temp(*this);
  Zero();

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      for (int k = 0; k < 3; ++k)
        m_[i][j] += temp.m_[i][k] * rhm.m_[k][j];

  return *this;
}

Mat2D & Mat2D::operator+=(const Mat2D & rhm)
{
  Mat2D temp(*this);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
        m_[i][j] = temp.m_[i][j] + rhm.m_[i][j];

  return *this;
}

float & Mat2D::operator()(int i, int j)
{
  return m_[i][j];
}

//Mat2D::Proxy Mat2D::operator[](int index)
//{
//  
//}

void Mat2D::Zero()
{
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      m_[i][j] = 0.0f;
}

void Mat2D::Transpose()
{
  Mat2D temp(*this);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      if (i != j)
        m_[i][j] = temp.m_[j][i];
}

//Mat2D::Proxy::Proxy(Mat2D & mat, int i) : value_(mat), i_(i)
//{
//}
//
//float Mat2D::Proxy::operator=(float f)
//{
//  return 0.0f;
//}
//
//float Mat2D::Proxy::operator=(const Proxy & p)
//{
//  return 0.0f;
//}
//
//Mat2D::Proxy::operator float() const
//{
//}
