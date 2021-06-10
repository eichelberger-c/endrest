///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Mat2D.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: The matrix class we'll convert to eventually hopefully 
//  
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
#include "Vec2D.h"


///----------------------------------GLOBALS------------------------------------


///----------------------------------CLASSES------------------------------------
class Mat2D
{
public:
  Mat2D(); //identity matrix
  Mat2D(const Mat2D& ogMat);
  Mat2D(float angle); //rotation matrix (degrees)
  Mat2D(float angle, Vec2D offset); //rotation offset from origin matrix
  Mat2D(Vec2D translation); //translation matrix
  Mat2D(Vec2D& scale); //scaling matrix (reference to differentiate between scale and translation for compiler)

  void Zero(); //sets all values in matrix to 0
  void Transpose();
  Mat2D& LightRotation(float angle);

  Mat2D& operator~(); //turns into identity matrix
  bool operator==(const Mat2D& rhm);
  bool operator!=(const Mat2D& rhm);
  Mat2D operator+(const Mat2D& rhm);
  Mat2D operator-(const Mat2D& rhm);
  Mat2D operator*(const Mat2D& rhm);
  Mat2D operator*(float scalar);
  Mat2D& operator*=(const Mat2D& rhm);
  Mat2D& operator+=(const Mat2D& rhm);
  float& operator()(int i, int j);

  //class Proxy
  //{
  //  Proxy(Mat2D& mat, int i);
  //  float operator=(float i);
  //  float operator=(const Proxy& p);
  //  operator float() const;

  //  Mat2D& value_;
  //  int i_;
  //  int j_;
  //};

  //Proxy operator[](int index);

  friend class Vec2D;

private:
  float m_[3][3];
};

///---------------------------------FUNCTIONS-----------------------------------


