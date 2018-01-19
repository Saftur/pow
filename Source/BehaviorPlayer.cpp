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
#include "BehaviorPlayer.h"
#include "GameObjectManager.h"
#include "Vector2D.h"
#include "GameStateManager.h"
#include "PlatformManager.h"
#include "BehaviorCheckpoint.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

enum PlayerBehaviors {cPlayerInvalid, cPlayerIdle};

float BehaviorPlayer::playerSpeedModifier = 0.0f;
const float BehaviorPlayer::playerSpeedMax = 500.0f;
const float BehaviorPlayer::playerJumpAcceleration = 150.0f;
const float BehaviorPlayer::playerJumpSpeedMax = 250.0f;
float BehaviorPlayer::playerGravity = 200.0f;
bool BehaviorPlayer::jumping = false;
bool BehaviorPlayer::inAir = false;
bool BehaviorPlayer::wasGrounded = false;
const float BehaviorPlayer::maxPlayerTime = 1.0f;
float BehaviorPlayer::touchTime = 0.0f;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void BehaviorPlayer::UpdateVelocity(Behavior& behavior, float dt)
{
	Transform* t = behavior.parent.GetTransform();
	Physics* p = behavior.parent.GetPhysics();

	if (!p || !t)
		return;

	Vector2D vel = p->GetVelocity();

	if(AEInputCheckCurr('A') || AEInputCheckCurr(VK_LEFT)) vel.X(-(playerSpeedMax + playerSpeedModifier));
	else if (AEInputCheckCurr('D') || AEInputCheckCurr(VK_RIGHT)) vel.X(playerSpeedMax + playerSpeedModifier);
	else vel.X(0);

	///TODO: Replace 'grounded' with a function call to determine if the object is on a platform.
	Vector2D newPos = Vector2D();
	bool grounded = PlatformManager::IsOnPlatform(&behavior.parent, &newPos);
	//If a jump button is pressed state that we are jumping and no longer grounded.
	if (AEInputCheckTriggered('W') || AEInputCheckTriggered(VK_SPACE) || AEInputCheckTriggered(VK_UP)) {
		if (grounded) {
			jumping = true;
			inAir = true;
			playerSpeedModifier += 15.0f;
		}
	}

	//If we are jumping increase upward velocity until we release jump button or hit max jump speed.
	if (jumping) {
		vel.Y(vel.Y() + (playerJumpAcceleration + playerSpeedModifier) * dt);
		if (AEInputCheckReleased('W') || AEInputCheckReleased(VK_SPACE) || AEInputCheckReleased(VK_UP)) jumping = false;
		if (vel.Y() >= playerJumpSpeedMax + playerSpeedModifier) jumping = false;
	}

	//If we are in the air but not jumping, apply gravity.
	if(inAir && !jumping) {
		vel.Y(vel.Y() - playerGravity * dt);
	}

	//If we just landed on a platform.
	if (grounded && inAir && !jumping) {
		inAir = false;
		vel.Y(0);
		behavior.parent.GetTransform()->SetTranslation(newPos);
	}

	if(!grounded) inAir = true;

	if (vel.X() > playerSpeedMax) vel.X(playerSpeedMax);

	p->SetVelocity(vel);

	// If we are touching a platform and were last frame, incrment a counter.
	if (wasGrounded && grounded)
	{
		touchTime += dt;

		if (touchTime > maxPlayerTime)
		{
			BehaviorCheckpoint::ResetUnconditional();
		}
	}
	else
	{
		touchTime = 0;
	}

	wasGrounded = grounded;
}

// Dynamically allocate a new (Spaceship) behavior component.
BehaviorPlayer::BehaviorPlayer(GameObject& parent)
	: base(parent)
{
	base.stateCurr = cPlayerInvalid;
	base.stateNext = cPlayerIdle;
	base.onInit = BehaviorPlayer::Init;
	base.onUpdate = BehaviorPlayer::Update;
	base.onExit = BehaviorPlayer::Exit;
	base.clone = BehaviorPlayer::Clone;
	base.destroy = BehaviorPlayer::Destroy;
}

BehaviorPlayer::BehaviorPlayer(const Behavior& other, GameObject& parent)
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
void BehaviorPlayer::CollisionHandler(GameObject& player, GameObject& other)
{
	UNREFERENCED_PARAMETER(player);
	UNREFERENCED_PARAMETER(other);
	///TODO: Reset player if they hit the ground below the platforms.
}

// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Behavior* BehaviorPlayer::Clone(const Behavior& behavior, GameObject& parent)
{
	return (Behavior*) new BehaviorPlayer(behavior, parent);
}

// Destroy an advanced behavior.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
void BehaviorPlayer::Destroy(Behavior& behavior)
{
	delete (BehaviorPlayer*) &behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorPlayer::Init(Behavior& behavior)
{
	if (behavior.stateCurr == cPlayerIdle)
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
void BehaviorPlayer::Update(Behavior& behavior, float dt)
{
	if (behavior.stateCurr == cPlayerIdle) {
		UpdateVelocity(behavior, dt);
	}
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorPlayer::Exit(Behavior& behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------


