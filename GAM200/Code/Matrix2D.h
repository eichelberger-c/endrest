#pragma once

///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Matrix2D.h
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//	Brief: This file contains the implementation of the Matrix2D class.  It 
//			conatins all of the old CS230 style comments which I will change 
//			before milestone 1.
//  
///-----------------------------USE-OR-DIE-THANKS-------------------------------


///----------------------------------INCLUDES-----------------------------------
#include "Vec2D.h"

///----------------------------------GLOBALS------------------------------------
//#define Matrix2DRowCol(pMtx, row, col) (pMtx)->m[(row)][(col)]

	typedef struct Matrix
	{
		// The matrix is stored in column-major format, meaning the consecutive
		//   elements of columns are contiguous.
		//   m[row][column]
		float m[3][3];
    int rows;
    int cols;

	}Matrix;

///----------------------------------CLASSES------------------------------------
class Matrix2D
{
public:

///---------------------------------FUNCTIONS-----------------------------------

// This constructor creates an identity matrix.
	Matrix2D();
  //Matrix2D(float angle, Vec2D offset); //makes a rotation offset from origin matrix

	// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
	// (NOTE: Care must be taken when pResult = pMtx.)
	void Matrix2DTranspose();

	// This function creates a translation matrix from x & y and saves it in Result.
	Matrix2D& Matrix2DTranslate(float x, float y);

	// This function creates a scaling matrix from x & y and saves it in Result.
	Matrix2D& Matrix2DScale(float x, float y);

	// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
	// Save the resultant matrix in Result.
	// Converting from degrees to radians can be performed as follows:
	//	 radians = (angle * M_PI) / 180.0f
	// M_PI is defined in "math.h", which may be included as follows:
	//   #define _USE_MATH_DEFINES
	//   #include <math.h>
	Matrix2D& Matrix2DRotDeg(float angle);

	// This matrix creates a rotation matrix from "Angle" whose value is in radians.
	// Save the resultant matrix in Result.
	Matrix2D& Matrix2DRotRad(float angle);
  Matrix2D& Matrix2DRotRadLight(float angle);

	Vec2D& operator*(Vec2D *vec1);

	// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
	// Result = Mtx0*Mtx1
	// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
	void operator*=(Matrix2D pMtx1);

	Matrix2D& operator=(Matrix2D *rhm);

  Matrix GetMatrix() { return Matrix_; }

  float& Index(unsigned int row, unsigned int col);

  friend class Vector2D;

private:
	Matrix Matrix_;
};
