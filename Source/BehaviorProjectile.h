//------------------------------------------------------------------------------
//
// File Name:	BehaviorProjectile.h
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
#include "Behavior.h"

typedef class BehaviorArmy BehaviorArmy;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

// An example of the enums to be defined in BehaviorProjectile.cpp.
#if 0
enum ProjectileState
{
};
#endif

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorProjectile.h
class BehaviorProjectile : public Behavior
{
public:
	enum ProjectileTypes {pTypeInvisible, pTypeLaser, pTypeMissile};

	static GameObject* Projectiles[3];

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new (Projectile) behavior component.
	// Params:
	//  parent = The object that owns this behavior.
	BehaviorProjectile();

	// Fire the projectile towards the given target vector at the given speed.
	// Bullet will be assumed to have "missed" after lifetime seconds.
	void Fire(BehaviorArmy* army, Vector2D direction, int damage, int range);

private:
	// Clone an advanced behavior and return a pointer to the cloned object.
	// Params:
	//   behavior = Reference to the behavior that will be destroyed.
	//   parent = A reference to the parent object (the object that owns this component).
	// Returns:
	//   A pointer to an advanced behavior.
	Component* Clone() const;

	// Initialize the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	void OnEnter();

	// Update the current state of the behavior component.
	// (Hint: Refer to the lecture notes on finite state machines (FSM).)
	// Params:
	//	 behavior = Pointer to the behavior component.
	//	 dt = Change in time (in seconds) since the last game loop.
	void OnUpdate(float dt);

	void OnExit();

	// The collision handling function for Projectiles.
	// Params:
	//	 projectile = The projectile object.
	//	 other = The object the asteroid is colliding with.
	static void CollisionHandler(GameObject& projectile, GameObject& other);

	BehaviorArmy* GetArmy();

	void Load(rapidjson::Value& obj);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	struct
	{
		int damage;
		float speed;

		// TODO: Sound pointers and such
	} projectile;

	BehaviorArmy* army;
	Vector2D target;
	float lifetime; // Range * speed
};

//------------------------------------------------------------------------------
