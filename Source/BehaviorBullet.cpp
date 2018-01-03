//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.cpp
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
#include "BehaviorBullet.h"
#include "GameObject.h"
#include "Teleporter.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

enum BulletBehaviors {cBulletInvalid, cBulletIdle};

const float BehaviorBullet::bulletSpeedMax = 500.0f;
const float BehaviorBullet::bulletLifeTimeMax = 3.0f;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new Bullet behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorBullet::BehaviorBullet(GameObject& parent)
	: base(parent)
{
	base.stateCurr = cBulletInvalid;
	base.stateNext = cBulletIdle;
	base.onInit = Init;
	base.onUpdate = Update;
	base.onExit = Exit;
	base.timer = bulletLifeTimeMax;
	base.clone = BehaviorBullet::Clone;
	base.destroy = BehaviorBullet::Destroy;
}

// Copy an existing bullet behavior component.
// Params:
//  other  = The behavior being copied.
//  parent = The object that owns this behavior.
BehaviorBullet::BehaviorBullet(const Behavior& other, GameObject& parent)
	: base(parent)
{
	base.stateCurr = other.stateCurr;
	base.stateNext = other.stateNext;
	base.onInit = other.onInit;
	base.onUpdate = other.onUpdate;
	base.onExit = other.onExit;
	base.timer = other.timer;
	base.clone = other.clone;
	base.destroy = other.destroy;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Behavior* BehaviorBullet::Clone(const Behavior& behavior, GameObject& parent)
{
	return (Behavior*) new BehaviorBullet(behavior, parent);
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void BehaviorBullet::Destroy(Behavior& behavior)
{
	delete (BehaviorBullet*) &behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorBullet::Init(Behavior& behavior)
{
	if (behavior.stateCurr == cBulletIdle)
	{
		if (behavior.parent.GetCollider())
			behavior.parent.GetCollider()->SetCollisionHandler(CollisionHandler);
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBullet::Update(Behavior& behavior, float dt)
{
	switch (behavior.stateCurr)
	{
	case cBulletInvalid:
		break;
	case cBulletIdle:
		BehaviorBullet::UpdateLifeTimer(behavior, dt);
		break;
	}

	TeleporterUpdateObject(behavior.parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBullet::Exit(Behavior& behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// Decrease the life timer and destroy the bullet. 
void BehaviorBullet::UpdateLifeTimer(Behavior& behavior, float dt)
{
	if (behavior.timer > 0)
	{
		behavior.timer -= dt;
	}
	else
		behavior.parent.Destroy();
}

// The collision handling function for bullets.
// Params:
//	 bullet = The bullet object.
//	 other = The object the bullet is colliding with.
void BehaviorBullet::CollisionHandler(GameObject& bullet, GameObject& other)
{
	if (other.IsNamed("Asteroid"))
		bullet.Destroy();
}

//------------------------------------------------------------------------------
