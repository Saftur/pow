//------------------------------------------------------------------------------
//
// File Name:	BehaviorAsteroid.cpp
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
#include "GameStateAsteroids.h"
#include "BehaviorAsteroid.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
enum AsteroidState {cAsteroidInvalid, cAsteroidIdle };
enum AsteroidSize { cAsteroidSmall, cAsteroidMedium, cAsteroidLarge };
//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (Asteroid) behavior component.
// Params:
//  parent = The object that owns this behavior.
BehaviorAsteroid::BehaviorAsteroid(GameObject& parent)
	: base(parent)
{
	base.stateCurr = cAsteroidInvalid;
	base.stateNext = cAsteroidIdle;
	base.onInit = BehaviorAsteroid::Init;
	base.onUpdate = BehaviorAsteroid::Update;
	base.onExit = BehaviorAsteroid::Exit;
	base.clone = BehaviorAsteroid::Clone;
	base.destroy = BehaviorAsteroid::Destroy;
	size = cAsteroidLarge;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Copy an existing asteroid behavior component.
// Params:
//  other  = The behavior being copied.
//  parent = The object that owns this behavior.
BehaviorAsteroid::BehaviorAsteroid(const Behavior& other, GameObject& parent)
	: base(parent)
{
	base.stateCurr = other.stateCurr;
	base.stateNext = other.stateNext;
	base.onInit = other.onInit;
	base.onUpdate = other.onUpdate;
	base.onExit = other.onExit;
	base.clone = other.clone;
	base.destroy = other.destroy;
	size = ((const BehaviorAsteroid&)other).size;
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Behavior* BehaviorAsteroid::Clone(const Behavior& behavior, GameObject& parent)
{
	return (Behavior*) new BehaviorAsteroid(behavior, parent);
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void BehaviorAsteroid::Destroy(Behavior& behavior)
{
	delete (BehaviorAsteroid*)&behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorAsteroid::Init(Behavior& behavior)
{
	switch (behavior.stateCurr)
	{
	case cAsteroidIdle:
		((BehaviorAsteroid&)behavior).origin = RandomRange(0, 3);
		((BehaviorAsteroid&)behavior).SetPosition();
		((BehaviorAsteroid&)behavior).SetVelocity();

		if (behavior.parent.GetCollider())
			behavior.parent.GetCollider()->SetCollisionHandler(CollisionHandler);
		break;
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorAsteroid::Update(Behavior& behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	switch (behavior.stateCurr)
	{
	case cAsteroidInvalid:
		break;
	case cAsteroidIdle:
		break;
	}

	TeleporterUpdateObject(behavior.parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorAsteroid::Exit(Behavior& behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// The collision handling function for Asteroids.
// Params:
//	 asteroid = The asteroid object.
//	 other = The object the asteroid is colliding with.
void BehaviorAsteroid::CollisionHandler(GameObject& asteroid, GameObject& other)
{
	if (other.IsNamed("Bullet") || other.IsNamed("Spaceship"))
	{
		GameStateAsteroids::IncreaseScore(10);
	 	((BehaviorAsteroid*)asteroid.GetBehavior())->SpawnNewAsteroids();
		asteroid.Destroy();
	}
}

// Set position of asteroid based on origin
void BehaviorAsteroid::SetPosition()
{
	switch (size)
	{
	case cAsteroidLarge:
		switch (origin)
		{
		case 0:
			base.parent.GetTransform()->SetTranslation(Vector2D((float)AEGfxGetWinMinX(), (float)AEGfxGetWinMinY()));
			break;
		case 1:
			base.parent.GetTransform()->SetTranslation(Vector2D((float)AEGfxGetWinMaxX(), (float)AEGfxGetWinMinY()));
			break;
		case 2:
			base.parent.GetTransform()->SetTranslation(Vector2D((float)AEGfxGetWinMinX(), (float)AEGfxGetWinMaxY()));
			break;
		case 3:
			base.parent.GetTransform()->SetTranslation(Vector2D((float)AEGfxGetWinMaxX(), (float)AEGfxGetWinMaxY()));
			break;
		}
		break;
	}
}

// Set velocity based on size
void BehaviorAsteroid::SetVelocity()
{
	float angle = 0.0f;
	switch (size)
	{
	case cAsteroidLarge:
		switch (origin)
		{
		case 0:
			angle = RandomRangeFloat(-10, -80);
			break;
		case 1:
			angle = RandomRangeFloat(-100, -170);
			break;
		case 2:
			angle = RandomRangeFloat(10, 80);
			break;
		case 3:
			angle = RandomRangeFloat(100, 170);
			break;
		}
		break;
	case cAsteroidMedium:
	case cAsteroidSmall:
		angle = RandomRangeFloat(0, 359);
		break;
	}

	base.parent.GetPhysics()->SetVelocity(Vector2D::FromAngleDegrees(angle) * RandomRangeFloat(asteroidSpeedMin, asteroidSpeedMax));
}

// Generate new asteroids based off this asteroid
void BehaviorAsteroid::SpawnNewAsteroids()
{
	float scale = 30.0f;
	int size2 = cAsteroidMedium;

	switch (size)
	{
	case cAsteroidMedium:
		size2 = cAsteroidSmall;
		scale = 10.0f;
	case cAsteroidLarge:
		for (int i = 0; i < 2; i++)
		{
			GameObject* newAst = new GameObject(base.parent);
			newAst->GetTransform()->SetScale(Vector2D(scale, scale));
			((BehaviorAsteroid*)newAst->GetBehavior())->SetVelocity();
			((BehaviorAsteroid*)newAst->GetBehavior())->size = size2;
			newAst->GetBehavior()->stateCurr = cAsteroidIdle;
			GameObjectManager::GetInstance().Add(*newAst);
		}
		break;
	case cAsteroidSmall:
		break;
	}
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

const float BehaviorAsteroid::asteroidSpeedMin = 50.0f;
const float BehaviorAsteroid::asteroidSpeedMax = 100.0f;

//------------------------------------------------------------------------------
