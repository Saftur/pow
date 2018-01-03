//------------------------------------------------------------------------------
//
// File Name:	Collider.h
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef void(*CollisionEventHandler)(GameObject& gameObject1, GameObject& gameObject2);

// An example of the structure to be defined in Collider.c.
class Collider
{
public:
	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new collider component.
	Collider(GameObject& parent);

	// Disable copy constructor
	Collider(const Collider& other) = delete;

	// Copy an existing collider component.
	// (Hint: Perform a shallow copy of the member variables.)
	// Params:
	//	 other = Reference to the component to be cloned.
	//   parent = Reference to the object that owns this collider.
	Collider(const Collider& other, GameObject& parent);

	// Check if two objects are colliding.
	// (Hint: Refer to the project instructions for implementation suggestions.)
	// (Hint: Make sure to call the handler for both colliders, passing the 
	//	  parent game object references in the correct order!)
	// Params:
	//	 other = Reference to the second collider component.
	void CheckCollision(const Collider& other);

	// Set the collision event handler for a collider.
	// (Hint: This allows other components, such as behaviors, to respond to collision events.)
	// (Note: It is acceptable for the handler to be nullptr.  This allows an existing handler to be removed.)
	// Params:
	//	 handler = Pointer to the collision event handler (may be nullptr).
	void SetCollisionHandler(CollisionEventHandler handler);

private:

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	float radius;

	// Pointer to the collider's parent game object.
	GameObject& parent;

	// Pointer to a function that handles collisions between two objects.
	CollisionEventHandler handler;
};

//------------------------------------------------------------------------------
