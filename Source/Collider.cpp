//------------------------------------------------------------------------------
//
// File Name:	Collider.cpp
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
#include "Collider.h"
#include "GameObject.h"
#include "AEEngine.h"

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
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new collider component.
Collider::Collider(GameObject& parent)
	: parent(parent), radius(parent.GetTransform()->GetScale().X() / 2.0f), handler(NULL) {}

// Copy an existing collider component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Reference to the component to be cloned.
//   parent = Reference to the object that owns this collider.
Collider::Collider(const Collider& other, GameObject& parent)
	: parent(parent), radius(parent.GetTransform()->GetScale().X()), handler(other.handler) {}

// Check if two objects are colliding.
// (Hint: Refer to the project instructions for implementation suggestions.)
// (Hint: Make sure to call the handler for both colliders, passing the 
//	  parent game object references in the correct order!)
// Params:
//	 other = Reference to the second collider component.
void Collider::CheckCollision(const Collider& other)
{
	if (parent.GetTransform()->GetTranslation().Distance(other.parent.GetTransform()->GetTranslation()) < radius + other.radius)
	{
		if (handler)
			handler(parent, other.parent);
		if (other.handler)
			other.handler(other.parent, parent);
	}
}

// Set the collision event handler for a collider.
// (Hint: This allows other components, such as behaviors, to respond to collision events.)
// (Note: It is acceptable for the handler to be nullptr.  This allows an existing handler to be removed.)
// Params:
//	 handler = Pointer to the collision event handler (may be nullptr).
void Collider::SetCollisionHandler(CollisionEventHandler theHandler)
{
	handler = theHandler;
}

//------------------------------------------------------------------------------
