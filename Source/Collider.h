//------------------------------------------------------------------------------
//
// File Name:	Collider.h
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S18
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h"
#include "Transform.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum ColliderType
{
	ColliderTypeNone,
	ColliderTypeCircle,
	ColliderTypeBox,
} ColliderType;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef void(*CollisionEventHandler)(GameObject& gameObject1, GameObject& gameObject2);

class Collider : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new collider component.
	// Params:
	//   parent = Reference to the object that owns this component.
	//   type = The type of collider (circle, line, etc.).
	Collider(ColliderType type);

	Collider *NewCollider(ColliderType type);

	// Clone an collider and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a collider.
	virtual Component* Clone() const = 0;

	// Debug drawing for colliders.
	virtual void Draw() const;

	virtual void SetParent(GameObject* object);

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

	// Perform intersection test between two arbitrary colliders.
	// Params:
	//	 other = Reference to the second collider component.
	virtual bool IsCollidingWith(const Collider& other) const = 0;

	// Get the type of this component.
	ColliderType GetType() const;

	virtual void Load(rapidjson::Value& obj);

	const Transform& GetTransform() const;
	void SetTransform(const Transform trs);

protected:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	Transform transform;

private:
	// The type of collider used by this component.
	// (Currently, Circle or Line).
	ColliderType type;

	// Pointer to a function that handles collisions between two objects.
	CollisionEventHandler handler;
};

//------------------------------------------------------------------------------
