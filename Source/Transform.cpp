/*******************************************************************************
Filename: Transform.cpp
Author: Mark Culp (mark.culp)
Course: CS230
Project: CS230 Project 4
Date: 9 November 2017
Description: Implementation of Transform.h functions and stuff.

******************************************************************************/

#include "stdafx.h"
#include "Transform.h"


Vector2D Transform::camTranslation = { 0, 0 };

// Create a new transform object.
// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
// Params:
//	 x = Initial world position on the x-axis.
//	 y = Initial world position on the y-axis.
Transform::Transform(float x, float y)
	: translation(Vector2D(x, y)), rotation(0.0f), scale(Vector2D(1, 1)), matrix(Matrix2D()), isDirty(true) {}

// Get the transform matrix, based upon translation, rotation and scale settings.
// Returns:
//	 A reference to the component's matrix structure
Matrix2D& Transform::GetMatrix()
{
	if (isDirty)
	{
		Matrix2D newTranslation = Matrix2D().TranslationMatrix(translation.X(), translation.Y());
		Matrix2D newScale = Matrix2D().ScalingMatrix(scale.X(), scale.Y());
		Matrix2D newRotation = Matrix2D().RotationMatrixRadians(rotation);

		matrix = newTranslation * newRotation * newScale;

		isDirty = false;
	}

	matrix = Matrix2D().TranslationMatrix(camTranslation.X(), camTranslation.Y()) * matrix;

	return  matrix;
}

// Get the translation of a transform component.
// Returns:
//	 A reference to the component's translation structure.
const Vector2D& Transform::GetTranslation() const
{
	return translation;
}

// Get the rotation value of a transform component.
// Returns:
//	 The component's rotation value (in radians).
float Transform::GetRotation() const
{
	return rotation;
}

// Get the scale of a transform component.
// Returns:
//	 A reference to the component's scale structure.
const Vector2D& Transform::GetScale() const
{
	return scale;
}

// Set the translation of a transform component.
// Params:
//	 translation = Reference to a translation vector.
void Transform::SetTranslation(const Vector2D& translation8)
{
	translation = translation8;
	isDirty = true;
}

// Set the rotation of a transform component.
// Params:
//	 rotation = The rotation value (in radians).
void Transform::SetRotation(float rotation7)
{
	rotation = rotation7;
	isDirty = true;
}

// Set the scale of a transform component.
// Params:
//	 translation = Reference to a scale vector.
void Transform::SetScale(const Vector2D& scale6)
{
	scale = scale6;
	isDirty = true;
}

void Transform::SetCamTranslation(const Vector2D & translation)
{
	camTranslation = translation;
}

Vector2D Transform::GetCamTranslation()
{
	return camTranslation;
}
