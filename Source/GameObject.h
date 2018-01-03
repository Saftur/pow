//------------------------------------------------------------------------------
//
// File Name:	GameObject.h
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

#include "Animation.h"
#include "Behavior.h"
#include "Collider.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in GameObject.c.
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class GameObject
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new game object.
	// Params:
	//	 name = The name of the game object being created.
	GameObject(const char * name);

	// Clone a game object from another game object.
	// Params:
	//	 other = A reference to the object being cloned.
	GameObject(const GameObject& other);

	// Free the memory associated with a game object.
	~GameObject();

	// Attach an animation component to a game object.
	// Params:
	//   animation = Reference to the animation component to be attached.
	void SetAnimation(Animation& animation);

	// Attach a behavior component to a game object.
	// Params:
	//   behavior = Reference to the behavior component to be attached.
	void SetBehavior(Behavior& behavior);

	// Attach a collider component to a game object.
	// Params:
	//   collider = Reference to the collider component to be attached.
	void SetCollider(Collider& collider);

	// Attach a physics component to a game object.
	// Params:
	//   physics = Reference to the physics component to be attached.
	void SetPhysics(Physics& physics);

	// Attach a sprite component to a game object.
	// Params:
	//   sprite = Reference to the sprite component to be attached.
	void SetSprite(Sprite& sprite);

	// Attach a transform component to a game object.
	// Params:
	//   transform = Reference to the transform component to be attached.
	void SetTransform(Transform& transform);

	// Get the animation component attached to a game object.
	// Returns:
	//		A pointer to the attached animation component.	
	Animation* GetAnimation();

	// Get the behavior component attached to a game object.
	// Returns:
	//		A pointer to the attached behavior component.	
	Behavior* GetBehavior();

	// Get the collider component attached to a game object.
	// Returns:
	//		A pointer to the attached collider component.	
	Collider* GetCollider();

	// Get the physics component attached to a game object.
	// Returns:
	//		A pointer to the attached physics component.
	Physics* GetPhysics();

	// Get the sprite component attached to a game object.
	// Returns:
	//		A pointer to the attached sprite component.
	Sprite* GetSprite();

	// Get the transform component attached to a game object.
	// Returns:
	//		A pointer to the attached transform component.
	Transform* GetTransform();

	// Get the name of the object.
	// Returns:
	//		A string literal containing the name of the object.	
	const char* GetName() const;

	// Check to see if the name of an object equals a given string.
	// Params:
	//   name = The string to compare to the object name.
	// Returns:
	//		True if the names match, false otherwise.
	bool IsNamed(const char* name_) const;

	// Whether the object has been marked for destruction.
	// Returns:
	//		True if the object will be destroyed, false otherwise.
	bool IsDestroyed() const;

	// Update any components attached to the game object.
	// (Hint: You will need to call Update on the Animation and Physics components.)
	// (NOTE: You must first check for valid pointers before calling these functions.)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);

	// Draw any visible components attached to the game object.
	// (Hint: You will need to call Draw on the Sprite component, passing it the transform component.)
	// (NOTE: You must first check for valid pointers before calling this function.) 
	void Draw();

	// Mark an object for destruction.
	void Destroy();

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The name of the game object.
	const char * name;

	// Pointer to an attached animation component.
	Animation* animation;

	// Pointer to an attached behavior component.
	Behavior* behavior;

	// Pointer to an attached collider component.
	Collider* collider;

	// Pointer to an attached physics component.
	Physics* physics;

	// Pointer to an attached sprite component.
	Sprite* sprite;

	// Pointer to an attached transform component.
	Transform* transform;

	// Whether the object has been marked for destruction.
	bool isDestroyed;
};
//------------------------------------------------------------------------------
