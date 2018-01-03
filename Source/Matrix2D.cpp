/*******************************************************************************
Filename: Matrix2D.cpp
Author: Mark Culp (mark.culp)
Course: CS230
Project: CS230 Project 4
Date: 9 November 2017
Description: Implementation of Matrix2D.h functions and stuff.

******************************************************************************/
#include "stdafx.h"
#include "Matrix2D.h"

// Constructor for matrix. Sets all data in matrix to 0.
Matrix2D::Matrix2D() 
{
	m[0][0] = 0;
	m[0][1] = 0;
	m[0][2] = 0;
	m[1][0] = 0;
	m[1][1] = 0;
	m[1][2] = 0;
	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 0;
}

// This function creates an identity matrix and returns it.
Matrix2D Matrix2D::IdentityMatrix()
{
	Matrix2D returnVal = Matrix2D();

	returnVal.m[0][0] = 1;
	returnVal.m[1][1] = 1;
	returnVal.m[2][2] = 1;

	return returnVal;
}

// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
// (NOTE: Care must be taken when pResult = pMtx.)
Matrix2D Matrix2D::Transposed() const
{
	Matrix2D returnVal = Matrix2D();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			returnVal.m[i][j] = m[j][i];
		}
	}

	return returnVal;
}

// This function creates a translation matrix from x & y and returns it.
Matrix2D Matrix2D::TranslationMatrix(float x, float y)
{
	Matrix2D returnVal = IdentityMatrix();

	returnVal.m[0][2] = x;
	returnVal.m[1][2] = y;

	return returnVal;
}

// This function creates a scaling matrix from x & y and returns it.
Matrix2D Matrix2D::ScalingMatrix(float x, float y)
{
	Matrix2D returnVal = IdentityMatrix();

	returnVal.m[0][0] = x;
	returnVal.m[1][1] = y;

	return returnVal;
}

// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
// Return the resulting matrix.
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
Matrix2D Matrix2D::RotationMatrixDegrees(float angle)
{
	return RotationMatrixRadians((angle * (float)M_PI) / 180.0f);
}

// This matrix creates a rotation matrix from "Angle" whose value is in radians.
// Return the resulting matrix.
Matrix2D Matrix2D::RotationMatrixRadians(float angle)
{
	Matrix2D returnVal = Matrix2D();

	returnVal.m[0][0] = cos(angle);
	returnVal.m[0][1] = -sin(angle);
	returnVal.m[1][0] = sin(angle);
	returnVal.m[1][1] = cos(angle);
	returnVal.m[2][2] = 1;

	return returnVal;
}

// This function multiplies the current matrix with another and returns the result as a new matrix.
// HINT: Use the RowColumnMultiply function to help write this function.
Matrix2D Matrix2D::operator*(const Matrix2D& other) const
{
	Matrix2D returnVal = Matrix2D();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			returnVal.m[i][j] = RowColumnMultiply(other, i, j);
		}
	}

	return returnVal;
}

// This function multiplies the current matrix with another, storing the result in the current matrix.
// HINT: Use the RowColumnMultiply function to help write this function.
Matrix2D& Matrix2D::operator*=(const Matrix2D& other)
{
	Matrix2D imBothered = operator*(other);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m[i][j] = imBothered.m[i][j];
		}
	}

	return *this;
}

// This function multiplies a matrix with a vector and returns the result as a new vector.
// Result = Mtx * Vec.
Vector2D Matrix2D::operator*(const Vector2D& vec) const
{
	Matrix2D matrixVal = IdentityMatrix();
	Vector2D returnVal = Vector2D();
	
	matrixVal.m[0][2] = vec.X();
	matrixVal.m[1][2] = vec.Y();

	returnVal.X(RowColumnMultiply(matrixVal, 0, 2));
	returnVal.Y(RowColumnMultiply(matrixVal, 1, 2));

	return returnVal;
}

// Multiplies a row of the current matrix by a column in another and returns the result.
float Matrix2D::RowColumnMultiply(const Matrix2D& other, unsigned row, unsigned col) const
{
	float returnVal = 0;

	for (int i = 0; i < 3; i++)
	{
		returnVal += m[row][i] * other.m[i][col];
	}

	return returnVal;
}