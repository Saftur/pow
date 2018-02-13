//------------------------------------------------------------------------------
//
// File Name:	Physics.h
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Transform Transform;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in Physics.c.
class Physics : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new physics component.
	Physics();

	// Returns a dynamically allocated copy of the component.
	// Must be implemented so correct component is copied during copy.
	virtual Component* Clone() const;

	// Get the acceleration of a physics component.
	// Returns:
	//	 A reference to the component's acceleration structure.
	const Vector2D& GetAcceleration() const;

	// Get the velocity of a physics component.
	// Returns:
	//		A reference to the component's velocity structure.
	const Vector2D& GetVelocity() const;

	// Get the old translation (position) of a physics component.
	// Returns: 
	//		A reference to the component's oldTranslation structure,
	const Vector2D& GetOldTranslation() const;

	// Get the rotational velocity of a physics component.
	// Returns:
	//	 A float representing the new rotational velocity.
	float GetRotationalVelocity() const;

	// Set the acceleration of a physics component.
	// Params:
	//	 acceleration = Pointer to an acceleration vector.
	void SetAcceleration(const Vector2D& acceleration);

	// Set the velocity of a physics component.
	// Params:
	//	 velocity = Pointer to a velocity vector.
	void SetVelocity(const Vector2D& velocity);

	// Set the rotational velocity of a physics component.
	// Params:
	//	 velocity = New value for the rotational velocity.
	void SetRotationalVelocity(float velocity);

	// Update the state of a physics component.
	// (NOTE: This function must verify that the transform pointer is valid.)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);

	void Load(rapidjson::Value& obj);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Previous position.  May be used for resolving collisions.
	Vector2D	oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Rotational velocity, the speed and direction that the object rotates.
	float		rotationalVelocity;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	float		inverseMass;
};
//------------------------------------------------------------------------------
