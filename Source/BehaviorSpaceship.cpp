//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship::.cpp
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
#include "AEEngine.h"
#include "BehaviorSpaceship.h"
#include "GameObjectManager.h"
#include "Vector2D.h"
#include "Teleporter.h"
#include "GameStateManager.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

enum SpaceshipBehaviors {cSpaceshipInvalid, cSpaceshipIdle, cSpaceshipThrust, cSpaceshipDead};

const float BehaviorSpaceship::spaceshipAcceleration = 150.0f;
const float BehaviorSpaceship::spaceshipSpeedMax = 500.0f;
const float BehaviorSpaceship::spaceshipTurnRateMax = (float)M_PI / 1.5f;
const float BehaviorSpaceship::spaceshipWeaponCooldownTime = 0.25f;
const float BehaviorSpaceship::spaceshipWeaponBulletSpeed = 750.0f;
const float BehaviorSpaceship::spaceshipDeathDuration = 3.0f;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void BehaviorSpaceship::SpawnBullet(Behavior& behavior)
{
	GameObject* bulletArch = GameObjectManager::GetInstance().GetArchetype("Bullet");

	if (!bulletArch)
		return;

	GameObject* bullet = new GameObject(*bulletArch);
	Vector2D pos = behavior.parent.GetTransform()->GetTranslation();
	float rot = behavior.parent.GetTransform()->GetRotation();

	bullet->GetTransform()->SetTranslation(pos);
	bullet->GetTransform()->SetRotation(rot);

	Vector2D uv = Vector2D::FromAngleRadians(rot);

	bullet->GetPhysics()->SetVelocity(uv * spaceshipWeaponBulletSpeed);

	GameObjectManager::GetInstance().Add(*bullet);
}

void BehaviorSpaceship::UpdateRotation(Behavior& behavior)
{
	if (AEInputCheckCurr(VK_RIGHT))
		behavior.parent.GetPhysics()->SetRotationalVelocity(-spaceshipTurnRateMax);
	else if (AEInputCheckCurr(VK_LEFT))
		behavior.parent.GetPhysics()->SetRotationalVelocity(spaceshipTurnRateMax);
	else
		behavior.parent.GetPhysics()->SetRotationalVelocity(0);
}

void BehaviorSpaceship::UpdateVelocity(Behavior& behavior, float dt)
{
	Transform* t = behavior.parent.GetTransform();
	Physics* p = behavior.parent.GetPhysics();

	if (!p || !t)
		return;

	float rot = t->GetRotation();
	Vector2D rv = Vector2D::FromAngleRadians(rot);
	Vector2D vel = p->GetVelocity();

	vel += rv * spaceshipAcceleration * dt;
	float speed = vel.Magnitude();

	if (speed > spaceshipSpeedMax)
		vel *= spaceshipSpeedMax / speed;

	p->SetVelocity(vel);
}

void BehaviorSpaceship::UpdateWeapon(Behavior& behavior, float dt)
{
	if (behavior.timer > 0)
		behavior.timer -= dt;
	if (behavior.timer < 0)
		behavior.timer = 0;

	if (AEInputCheckCurr(VK_SPACE))
	{
		if (behavior.timer <= 0)
		{
			BehaviorSpaceship::SpawnBullet(behavior);
			behavior.timer = spaceshipWeaponCooldownTime;
		}
	}
}

// Dynamically allocate a new (Spaceship) behavior component.
BehaviorSpaceship::BehaviorSpaceship(GameObject& parent)
	: base(parent)
{
	base.stateCurr = cSpaceshipInvalid;
	base.stateNext = cSpaceshipIdle;
	base.onInit = BehaviorSpaceship::Init;
	base.onUpdate = BehaviorSpaceship::Update;
	base.onExit = BehaviorSpaceship::Exit;
	base.clone = BehaviorSpaceship::Clone;
	base.destroy = BehaviorSpaceship::Destroy;
}

BehaviorSpaceship::BehaviorSpaceship(const Behavior& other, GameObject& parent)
	: base(parent)
{
	base.stateCurr = other.stateCurr;
	base.stateNext = other.stateNext;
	base.onInit = other.onInit;
	base.onUpdate = other.onUpdate;
	base.onExit = other.onExit;
	base.clone = other.clone;
	base.destroy = other.destroy;
}

// The collision handling function for bullets, except it handles collisions as though it was a spaceship as a meme.
// Params:
//	 bullet = The bullet object.
//	 other = The object the bullet is colliding with.
void BehaviorSpaceship::CollisionHandler(GameObject& bullet, GameObject& other)
{
	if (other.IsNamed("Asteroid"))
		bullet.GetBehavior()->stateNext = cSpaceshipDead;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Behavior* BehaviorSpaceship::Clone(const Behavior& behavior, GameObject& parent)
{
	return (Behavior*) new BehaviorSpaceship(behavior, parent);
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void BehaviorSpaceship::Destroy(Behavior& behavior)
{
	delete (BehaviorSpaceship*) &behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorSpaceship::Init(Behavior& behavior)
{
	if (behavior.stateCurr == cSpaceshipIdle)
	{
		if (behavior.parent.GetCollider())
			behavior.parent.GetCollider()->SetCollisionHandler(CollisionHandler);
	}
	
	if (behavior.stateCurr == cSpaceshipDead)
	{
		UpdateDeathEffect(behavior);
		behavior.timer = spaceshipDeathDuration;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceship::Update(Behavior& behavior, float dt)
{
	switch (behavior.stateCurr)
	{
		case cSpaceshipInvalid:
			break;
		case cSpaceshipIdle:
			BehaviorSpaceship::UpdateRotation(behavior);
			BehaviorSpaceship::UpdateWeapon(behavior, dt);

			if (AEInputCheckCurr(VK_UP))
				behavior.stateNext = cSpaceshipThrust;
			break;
		case cSpaceshipThrust:
			BehaviorSpaceship::UpdateRotation(behavior);
			BehaviorSpaceship::UpdateVelocity(behavior, dt);
			BehaviorSpaceship::UpdateWeapon(behavior, dt);

			if (!AEInputCheckCurr(VK_UP))
				behavior.stateNext = cSpaceshipIdle;
			break;
		case cSpaceshipDead:
			if (behavior.timer > 0)
			{
				BehaviorSpaceship::UpdateDeathEffect(behavior);
				behavior.timer -= dt;
			}
			else
			{
				GameStateManager::GetInstance().SetNextState(GameStateTable::GsRestart);
			}
			break;
	}

	TeleporterUpdateObject(behavior.parent);
}

// Display a death effect.
// Params:
//	behavior = Pointer to the behavior component.
void BehaviorSpaceship::UpdateDeathEffect(Behavior& behavior)
{
	behavior.parent.GetTransform()->SetScale(behavior.parent.GetTransform()->GetScale() * 1.03f);
	behavior.parent.GetPhysics()->SetVelocity(Vector2D(0, 0));
	behavior.parent.GetPhysics()->SetRotationalVelocity(5);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceship::Exit(Behavior& behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------


