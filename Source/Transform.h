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

#include "Component.h"
#include "Vector2D.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in Transform.h.
// You are free to change the contents of this structure as long as you do not
//   change the public method declarations declared in the header.
class Transform : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Transform();

	// Create a new transform object.
	// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
	// Params:
	//	 x = Initial world position on the x-axis.
	//	 y = Initial world position on the y-axis.
	Transform(float x, float y);

	// Clones the transform component, returning a dynamically allocated copy.
	Component* Clone() const;

	// Get the transform matrix, based upon translation, rotation and scale settings.
	// Returns:
	//	 A reference to the component's matrix structure
	Matrix2D& GetMatrix();

	// Get the translation of a transform component.
	// Returns:
	//	 A reference to the component's translation structure.
	const Vector2D& GetTranslation() const;
	const Vector2D GetWorldTranslation() const;
	const Vector2D GetScreenTranslation() const;

	// Get the rotation value of a transform component.
	// Returns:
	//	 The component's rotation value (in radians).
	float GetRotation() const;
	float GetWorldRotation() const;
	float GetScreenRotation() const;

	// Get the scale of a transform component.
	// Returns:
	//	 A reference to the component's scale structure.
	const Vector2D& GetScale() const;
	const Vector2D GetWorldScale() const;
	const Vector2D GetScreenScale() const;

	// Set the translation of a transform component.
	// Params:
	//	 translation = Reference to a translation vector.
	void SetTranslation(const Vector2D& translation);
	void SetWorldTranslation(const Vector2D& translation);
	void SetScreenTranslation(const Vector2D& translation);

	// Set the rotation of a transform component.
	// Params:
	//	 rotation = The rotation value (in radians).
	void SetRotation(float rotation);
	void SetWorldRotation(float rotation);
	void SetScreenRotation(float rotation);

	// Set the scale of a transform component.
	// Params:
	//	 translation = Reference to a scale vector.
	void SetScale(const Vector2D& scale);
	void SetWorldScale(const Vector2D& scale);
	void SetScreenScale(const Vector2D& scale);

	void SetParent(Transform *transform);

	void SetFollowCamera(bool fc);

	void Load(rapidjson::Value& obj);


	static void SetCamTranslation(const Vector2D& translation);

	static Vector2D GetCamTranslation();

	static void SetCamIsDirty(bool isDirty);
	
private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	Transform *parent;

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
	static Vector2D camScale;
	static bool camIsDirty;
};
//------------------------------------------------------------------------------
