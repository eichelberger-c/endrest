///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Matrix2D.cpp
//
//  Author: Zack Keenly
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//    -Matrix2D();
//		-void Matrix2DTranspose();
//		-Matrix2D& Matrix2DTranslate(float x, float y);
//		-void Matrix2DScale(float x, float y);
//		-void Matrix2DRotDeg(Matrix2D *pResult, float angle);
//		-void Matrix2DRotRad(Matrix2D *pResult, float angle);
//		-Vec2D& operator*(Vec2D *vec1);
//		-Matrix2D& operator*(Matrix2D *pMtx1);
//		-Matrix2D& operator=(Matrix2D *rhm);
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#define _USE_MATH_DEFINES
#include <cmath>
#include "stdafx.h"
#include "Matrix2D.h"

///----------------------------------GLOBALS-----------------------------------


///---------------------------------FUNCTIONS-----------------------------------

///-------------------------------------
/// Brief: Constructor
Matrix2D::Matrix2D()
{
	int i, j; //Loop variables

			  //Set entire matrix to 0
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
			Matrix_.m[i][j] = 0; //Indexes to a specific cell of a matrix
	}

	//Set main diagonal to 1's
	Matrix_.m[0][0] = 1;
	Matrix_.m[1][1] = 1;
	Matrix_.m[2][2] = 1;

  Matrix_.rows = 3;
  Matrix_.cols = 3;
}

//Matrix2D::Matrix2D(float angle, Vec2D offset)
//{
//  float x = offset.x_;
//  float y = offset.y_;
//
//  Matrix_.m[0][0] = (float)cos(angle);  //Row 1, Column 1
//  Matrix_.m[0][1] = (float)-sin(angle); //Row 1, Column 2
//  if ((angle <= M_PI_2 && angle >= 0) || (angle <= 0 && angle >= -M_PI_2))
//  {
//    Matrix_.m[0][2] = (-x * (float)cos(angle)) + x + (y * (float)sin(angle));
//  }
//  if ((angle <= M_PI && angle >= M_PI_2) || (angle >= -M_PI && angle <= -M_PI_2))
//  {
//    Matrix_.m[0][2] = (-x * (float)cos(angle)) + -x + (y * (float)sin(angle));
//  }
//  Matrix_.m[1][2] = (-x * (float)sin(angle)) - (y * (float)cos(angle)) + y;
//  Matrix_.m[1][0] = (float)sin(angle);  //Row 2, Column 1 
//  Matrix_.m[1][1] = (float)cos(angle);  //Row 2, Column 2
//  Matrix_.m[2][2] = 1;                  //Row 3, Column 3
//}


// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
void Matrix2D::Matrix2DTranspose()
{
	Matrix2D temp;

	int i, j; //Loop variables
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
			temp.Matrix_.m[i][j] = Matrix_.m[j][i]; //Transpose values across main diagonal
	}
	Matrix_ = temp.Matrix_;
}

// This function creates a translation matrix from x & y.
Matrix2D& Matrix2D::Matrix2DTranslate(float x, float y)
{
	Matrix_.m[0][2] = x;
	Matrix_.m[1][2] = y;

	return *this;
}

// This function creates a scaling matrix from x & y and saves it in Result.
Matrix2D& Matrix2D::Matrix2DScale(float x, float y)
{
	Matrix_.m[0][0] = x;
	Matrix_.m[1][1] = y;
	Matrix_.m[2][2] = 1;

	return *this;
}

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// Save the resultant matrix in Result.
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
Matrix2D& Matrix2D::Matrix2DRotDeg(float angle)
{
	float radians = (angle * (float)M_PI) / 180.0f; //Convert to radians
	//Matrix2D temp;

													//Set Rotation matrix about the Z-Axis
	Matrix_.m[0][0] = (float)cos(radians);  //Row 1, Column 1
	Matrix_.m[0][1] = (float)-sin(radians); //Row 1, Column 2
	Matrix_.m[1][0] = (float)sin(radians);  //Row 2, Column 1
	Matrix_.m[1][1] = (float)cos(radians);  //Row 2, Column 2
	Matrix_.m[2][2] = 1;                    //Row 3, Column 3

	return *this;
}

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// Save the resultant matrix in Result.
Matrix2D& Matrix2D::Matrix2DRotRad(float angle)
{
	//Matrix2D temp;

	//Set Rotation matrix about the Z-Axis
	Matrix_.m[0][0] = (float)cos(angle);  //Row 1, Column 1
	Matrix_.m[0][1] = (float)-sin(angle); //Row 1, Column 2
	Matrix_.m[1][0] = (float)sin(angle);  //Row 2, Column 1 
	Matrix_.m[1][1] = (float)cos(angle);  //Row 2, Column 2
	Matrix_.m[2][2] = 1;                  //Row 3, Column 3

	return *this;
}

Matrix2D& Matrix2D::Matrix2DRotRadLight(float angle)
{
  //Matrix2D temp;
  float x = -32.0f;
  float y = 0.0f;

  Matrix_.m[0][0] = (float)cos(angle);  //Row 1, Column 1
  Matrix_.m[0][1] = (float)-sin(angle); //Row 1, Column 2
  if ((angle <= M_PI_2 && angle >= 0) || (angle <= 0 && angle >= -M_PI_2))
  {
    Matrix_.m[0][2] = (-x * (float)cos(angle)) + x + (y * (float)sin(angle));
  }
  if ((angle <= M_PI && angle >= M_PI_2) || (angle >= -M_PI && angle <= -M_PI_2))
  {
    Matrix_.m[0][2] = (-x * (float)cos(angle)) + -x + (y * (float)sin(angle));
  }
  Matrix_.m[1][2] = (-x * (float)sin(angle)) - (y * (float)cos(angle)) + y;
  Matrix_.m[1][0] = (float)sin(angle);  //Row 2, Column 1 
  Matrix_.m[1][1] = (float)cos(angle);  //Row 2, Column 2
  Matrix_.m[2][2] = 1;                  //Row 3, Column 3

  return *this;
}

// This function multiplies the matrix Mtx with the vector Vec and saves the result in Result.
// Temp = Mtx * Vec.

Vec2D& Matrix2D::operator*(Vec2D *vec1)
{
	float sumX = 0.0f;
	float sumY = 0.0f;

	sumX += Matrix_.m[0][0] * vec1->x_;
	sumX += Matrix_.m[0][1] * vec1->y_;
	sumX += Matrix_.m[0][2];

	sumY += Matrix_.m[1][0] * vec1->x_;
	sumY += Matrix_.m[1][1] * vec1->y_;
	sumY += Matrix_.m[1][2];

	vec1->Set(sumX, sumY);

	return *vec1;
}

// This function multiplies Mtx0 with Mtx1 and saves the result in Result.
// Result = Mtx0*Mtx1
// (NOTE: Care must be taken when pResult = either pMtx0 or pMtx1.)
void Matrix2D::operator*=(Matrix2D pMtx1)
{
	int i, j, k; //Loop variables
	Matrix temp = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //Initialize matrix

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < 3; k++)
				temp.m[i][j] += (Matrix_.m[i][k] * pMtx1.Matrix_.m[k][j]); //Matrix multiplication; row by column
		}
	}
	Matrix_ = temp;
}

Matrix2D& Matrix2D::operator=(Matrix2D *rhm)
{
	int i, j;
	Matrix2D temp;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			temp.Matrix_.m[i][j] = rhm->Matrix_.m[i][j];
		}
	}

	this->Matrix_ = temp.Matrix_;

	return *this;
}

float & Matrix2D::Index(unsigned int row, unsigned int col)
{
  return Matrix_.m[row][col];
}
