//------------------------------------------------------------------------------
//
// File Name:	Physics.cpp
// Author(s):	Mark Culp
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Physics.h"
#include "Transform.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Create a new physics component.
Physics::Physics()
	: oldTranslation(Vector2D()), acceleration(Vector2D()), velocity(Vector2D()), inverseMass(0.0f) {}

// Get the acceleration of a physics component.
// Returns:
//	 A reference to the component's acceleration structure.
const Vector2D& Physics::GetAcceleration() const
{
	return acceleration;
}

// Get the velocity of a physics component.
// Returns:
//		A reference to the component's velocity structure.
const Vector2D& Physics::GetVelocity() const
{
	return velocity;
}

// Get the old translation (position) of a physics component.
// Returns: 
//		A reference to the component's oldTranslation structure,
const Vector2D& Physics::GetOldTranslation() const
{
	return oldTranslation;
}

// Get the rotational velocity of a physics component.
// Returns:
//	 A float representing the new rotational velocity.
float Physics::GetRotationalVelocity() const
{
	return rotationalVelocity;
}

// Set the acceleration of a physics component.
// Params:
//	 acceleration = Reference to an acceleration vector.
void Physics::SetAcceleration(const Vector2D& anAcceleration)
{
	acceleration = anAcceleration;
}

// Set the velocity of a physics component.
// Params:
//	 velocity = Reference to a velocity vector.
void Physics::SetVelocity(const Vector2D& trashVelocity)
{
	velocity = trashVelocity;
}

// Set the rotational velocity of a physics component.
// Params:
//	 velocity = Float representing the new rotational velocity.
void Physics::SetRotationalVelocity(float theRotationalVelocity)
{
	rotationalVelocity = theRotationalVelocity;
}

// Update the state of a physics component.
// Params:
//	 transform = Reference to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void Physics::Update(Transform& someTransform, float dt)
{
	oldTranslation = someTransform.GetTranslation();

	velocity = acceleration * dt + velocity;
	someTransform.SetTranslation(velocity * dt + oldTranslation);
	someTransform.SetRotation(someTransform.GetRotation() + (rotationalVelocity * dt));
}
//------------------------------------------------------------------------------
