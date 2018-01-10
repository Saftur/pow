//------------------------------------------------------------------------------
//
// File Name:	Transform.h
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in Transform.h.
// You are free to change the contents of this structure as long as you do not
//   change the public method declarations declared in the header.
class Transform
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new transform object.
	// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
	// Params:
	//	 x = Initial world position on the x-axis.
	//	 y = Initial world position on the y-axis.
	Transform(float x, float y);

	// Get the transform matrix, based upon translation, rotation and scale settings.
	// Returns:
	//	 A reference to the component's matrix structure
	Matrix2D& GetMatrix();

	// Get the translation of a transform component.
	// Returns:
	//	 A reference to the component's translation structure.
	const Vector2D& GetTranslation() const;

	// Get the rotation value of a transform component.
	// Returns:
	//	 The component's rotation value (in radians).
	float GetRotation() const;

	// Get the scale of a transform component.
	// Returns:
	//	 A reference to the component's scale structure.
	const Vector2D& GetScale() const;

	// Set the translation of a transform component.
	// Params:
	//	 translation = Reference to a translation vector.
	void SetTranslation(const Vector2D& translation);

	// Set the rotation of a transform component.
	// Params:
	//	 rotation = The rotation value (in radians).
	void SetRotation(float rotation);

	// Set the scale of a transform component.
	// Params:
	//	 translation = Reference to a scale vector.
	void SetScale(const Vector2D& scale);
	
	void SetFollowCamera(bool fc);
	

	static void SetCamTranslation(const Vector2D& translation);

	static Vector2D GetCamTranslation();

	static void SetCamIsDirty(bool isDirty);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The translation (or world position) of a game object.
	Vector2D	translation;

	// The rotation (or orientation) of a game object.
	float	rotation;

	// The scale (or size) of a game object.
	// (Hint: this should be initialized to (1, 1).)
	Vector2D	scale;

	// The transformation matrix resulting from concatenating the 
	//	 matrix = Translation*Rotation*Scale matrices.
	Matrix2D	matrix;

	// True if the transformation matrix needs to be recalculated.
	// (Hint: this should be initialized to true.)
	bool	isDirty;

	bool followCamera;


	static Vector2D camTranslation;
	static bool camIsDirty;
};
//------------------------------------------------------------------------------
