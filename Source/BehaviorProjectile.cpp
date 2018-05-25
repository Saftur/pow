//------------------------------------------------------------------------------
//
// File Name:	BehaviorProjectile.cpp
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
#include "Random.h"
#include "AEEngine.h"
#include "Teleporter.h"
#include "BehaviorProjectile.h"
#include "Physics.h"
#include "GameObjectManager.h"
#include "BehaviorUnit.h"
#include "ColliderCircle.h"
#include "BehaviorArmy.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

enum states { cProjectileIdle, cProjectileMoving, cProjectileHit, cProjectileMiss };

GameObject* BehaviorProjectile::Projectiles[3];

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Projectile) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorProjectile::BehaviorProjectile() :
		Behavior("BehaviorProjectile")
{
	SetCurrentState(cBehaviorInvalid);
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Component* BehaviorProjectile::Clone() const
{
	return new BehaviorProjectile(*this);
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorProjectile::OnEnter()
{
	switch (GetCurrentState())
	{
	case cProjectileIdle:
		
		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorProjectile::OnUpdate(float dt)
{
	switch (GetCurrentState())
	{
	case cProjectileIdle:
		SetNextState(cProjectileMoving);
		break;
	case cProjectileMoving:
		GetParent()->GetComponent<ColliderCircle>()->SetCollisionHandler(&CollisionHandler);
		GetParent()->GetComponent<Physics>()->SetVelocity(target * projectile.speed);
		lifetime -= dt;

		if (lifetime <= 0)
		{
			SetNextState(cProjectileMiss);
		}
		break;
	case cProjectileMiss:
		GetParent()->Destroy();
	}
}

void BehaviorProjectile::OnExit()
{
	switch (GetCurrentState()) {
	case cProjectileIdle:
		break;
	}
}

// The collision handling function for Projectiles.
// Params:
//	 projectile = The projectile object.
//	 other = The object the asteroid is colliding with.
void BehaviorProjectile::CollisionHandler(GameObject& projectile, GameObject& other)
{
	if (other.GetComponent<BehaviorUnit>() && other.GetComponent<BehaviorUnit>()->GetArmy() != projectile.GetComponent<BehaviorProjectile>()->GetArmy())
	{
		other.GetComponent<BehaviorUnit>()->ModifyHP(-projectile.GetComponent<BehaviorProjectile>()->projectile.damage);
		projectile.Destroy();
	}
}

BehaviorArmy * BehaviorProjectile::GetArmy()
{
	return army;
}

void BehaviorProjectile::Load(rapidjson::Value& obj)
{
	if (obj.HasMember("Type") && obj["Type"].IsInt())
	{
		Projectiles[obj["Type"].GetInt()] = GetParent();
	}
	if (obj.HasMember("Speed") && obj["Speed"].IsFloat())
	{
		projectile.speed = obj["Speed"].GetFloat();
	}
}

// Fire the projectile towards the given target vector at the given speed.
// Bullet will be assumed to have "missed" after lifetime seconds.
void BehaviorProjectile::Fire(BehaviorArmy* aArmy, Vector2D aTarget, int damage, int range)
{
	GetParent()->GetComponent<Transform>()->SetRotation(atan2(aTarget.y, aTarget.x));
	army = aArmy;
	aTarget = aTarget.Normalized();
	target = aTarget;
	projectile.damage = damage;

	float height = (float)GridManager::GetInstance().tileHeight;
	float width = (float)GridManager::GetInstance().tileWidth;
	Vector2D distance = aTarget * (range * sqrtf(height * height + width * width));
	float d = sqrtf(distance.x * distance.x + distance.y * distance.y);

	lifetime = d/projectile.speed;

	SetNextState(cProjectileMoving);
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
