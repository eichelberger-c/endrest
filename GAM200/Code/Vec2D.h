///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Vec2D.h
//
//  Author: Sonya Toft
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Brief: 2D vector class with several operator overloads
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#pragma once
//#include "Matrix2D.h"
//#include "Mat2D.h"

///----------------------------------GLOBALS------------------------------------
typedef class Mat2D Mat2D;

///----------------------------------CLASSES------------------------------------
//class Transform;

class Vec2D
{
public:
  Vec2D();
  Vec2D(const Vec2D& og);
  Vec2D(float angle);
  Vec2D(float x, float y);

  void Set(float x, float y);
  float GetLength();
  Vec2D Abs() const;
  void Zero();
  bool IsInRange(const Vec2D& vec, float range);
  double Distance(const Vec2D& rhv);
  void Normalize();


  Vec2D& operator=(const Vec2D& rhv);
  Vec2D operator*(const Vec2D& rhv) const;
  Vec2D operator*(float scalar) const;
  Vec2D& operator*=(int scalar);
  Vec2D& operator*=(const Mat2D& mat);
  Vec2D& operator*=(float scalar);
  Vec2D operator+=(float rhv);
  Vec2D operator+=(const Vec2D & rhv);
  float operator%(const Vec2D& rhv) const;
  Vec2D& operator~();
  float operator^(const Vec2D& rhv) const;
  Vec2D operator+(const Vec2D& rhv) const;
  Vec2D operator-(const Vec2D& rhv) const;
  Vec2D operator+(float val) const;
  Vec2D operator-(float val) const;
  bool operator<(const Vec2D& rhv) const;
  bool operator>(const Vec2D& rhv) const;
  bool operator<=(const Vec2D& rhv) const;
  bool operator>=(const Vec2D& rhv) const;
  bool operator<=(float scalar) const;
  bool operator>=(float scalar) const;
  bool operator<(float scalar) const;
  bool operator>(float scalar) const; 
  bool operator==(Vec2D& rhv) const;
  bool operator==(float scalar) const;
  Vec2D operator-(float scalar);
  Vec2D& operator-();

  float X();
  float Y();
  void X(float x) { x_ = x; }
  void Y(float y) { y_ = y; }

  friend class CollisionManager;
  friend class Transform;
  friend class Physics;
  friend class Matrix2D;
  friend class Mat2D;
  friend class Sprite;
  friend class Collider;

private:
  float x_;
  float y_;
};

///---------------------------------FUNCTIONS-----------------------------------
