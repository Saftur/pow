//------------------------------------------------------------------------------
//
// File Name:	GameObject.h
// Author(s):	Mark Culp
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

#include "stdafx.h"
#include "GameObject.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new game object.
// Params:
//	 name = The name of the game object being created.
GameObject::GameObject(const char * name)
	: name(name), sprite(NULL), animation(NULL), behavior(NULL), physics(NULL), transform(NULL), collider(NULL) {}

// Clone a game object from another game object.
// Params:
//	 other = A reference to the object being cloned.
GameObject::GameObject(const GameObject& other)
	: animation(NULL), behavior(NULL), isDestroyed(false), physics(NULL), sprite(NULL), transform(NULL), collider(NULL), name(other.name)
{
	if (other.sprite)
		sprite = new Sprite(*other.sprite);
	if (other.animation)
		animation = new Animation(*other.animation, sprite);
	if (other.behavior)
		behavior = other.behavior->Clone(*this);
	if (other.physics)
		physics = new Physics(*other.physics);
	if (other.transform)
		transform = new Transform(*other.transform);
	if (other.collider)
		//collider = new Collider(*this);
		collider = new Collider(*other.collider, *this);
}

// Free the memory associated with a game object.
GameObject::~GameObject()
{
	if (animation)
		delete(animation);
	if (physics)
		delete(physics);
	if (sprite)
		delete(sprite);
	if (behavior)
		Behavior::Destroy(behavior);
	if (transform)
		delete(transform);
	if (collider)
		delete(collider);
}

// Attach an animation component to a game object.
// Params:
//   animation = Pointer to the animation component to be attached.
void GameObject::SetAnimation(Animation& theAnimation)
{
	animation = &theAnimation;
}

// Attach an behavior component to a game object.
// Params:
//   behavior = Reference to the behavior component to be attached.
void GameObject::SetBehavior(Behavior& theBehavior)
{
	behavior = &theBehavior;
}

// Attach a collider component to a game object.
// Params:
//   collider = Reference to the collider component to be attached.
void GameObject::SetCollider(Collider& theCollider)
{
	collider = &theCollider;
}

// Attach a physics component to a game object.
// Params:
//   physics = Pointer to the physics component to be attached.
void GameObject::SetPhysics(Physics& thePhysics)
{
	physics = &thePhysics;
}

// Attach a sprite component to a game object.
// Params:
//   sprite = Pointer to the sprite component to be attached.
void GameObject::SetSprite(Sprite& theSprite)
{
	sprite = &theSprite;
}

// Attach a transform component to a game object.
// Params:
//   transform = Pointer to the transform component to be attached.
void GameObject::SetTransform(Transform& theTransform)
{
	transform = &theTransform;
}

// Get the animation component attached to a game object.
// Returns:
//		A pointer to the attached animation component,	
Animation* GameObject::GetAnimation()
{
	return animation;
}

// Get the behavior component attached to a game object.
// Returns:
//		A pointer to the attached behavior component.	
Behavior* GameObject::GetBehavior()
{
	return behavior;
}

// Get the collider component attached to a game object.
// Returns:
//		A pointer to the attached collider component.	
Collider* GameObject::GetCollider()
{
	return collider;
}

// Get the physics component attached to a game object.
// Returns:
//		A pointer to the attached physics component,
Physics* GameObject::GetPhysics()
{
	return physics;
}

// Get the sprite component attached to a game object.
// Returns:
//		A pointer to the attached sprite component,
Sprite* GameObject::GetSprite()
{
	return sprite;
}

// Get the transform component attached to a game object.
// Returns:
//		A pointer to the attached transform component,
Transform* GameObject::GetTransform()
{
	return transform;
}

// Get the name of the object.
// Returns:
//		A string literal containing the name of the object.	
const char* GameObject::GetName() const
{
	return name;
}

// Check to see if the name of an object equals a given string.
// Params:
//   name = The string to compare to the object name.
// Returns:
//		True if the names match, false otherwise.
bool GameObject::IsNamed(const char* name_) const
{
	int count = 0;

	while (name_[count])
	{
		if (!name[count] || name_[count] != name[count])
		{
			return false;
		}

		count++;
	}

	return true;
}

// Whether the object has been marked for destruction.
// Returns:
//		True if the object will be destroyed, false otherwise.
bool GameObject::IsDestroyed() const
{
	if (isDestroyed)
		return true;

	return false;
}

// Update any components attached to the game object.
// (Hint: You will need to call AnimationUpdate() and PhysicsUpdate().)
// (NOTE: You must first check for valid pointers before calling these functions.)
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObject::Update(float dt)
{
	if (transform == NULL) 
		return;
	if (behavior != NULL)
		behavior->Update(dt);
	if (physics != NULL)
		physics->Update(*transform, dt);
	if (animation != NULL)
		animation->Update(dt);
}

// Draw any visible components attached to the game object.
// (Hint: You will need to call SpriteDraw(), passing the sprite and transform components.)
// (NOTE: You must first check for valid pointers before calling this function.) 
void GameObject::Draw()
{
	if (transform == NULL) return;
	if (sprite == NULL) return;

	sprite->Draw(*transform);
}

// Mark an object for destruction.
void GameObject::Destroy()
{
	isDestroyed = true;
}
//------------------------------------------------------------------------------
