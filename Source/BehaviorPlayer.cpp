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
const float BehaviorPlayer::playerJumpSpeedMax = 450.0f;
float BehaviorPlayer::playerGravity = 800.0f;
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

	Vector2D newPos = Vector2D();
	int hitSide = PlatformManager::HittingSide(&behavior.parent, &newPos);
	if (hitSide != 0) {
		t->SetTranslation(newPos);
		if (hitSide == 2 && vel.Y() > 0)
			vel.Y(0);
	}

	if(AEInputCheckCurr('A') || AEInputCheckCurr(VK_LEFT) && hitSide != 1) vel.X(-(playerSpeedMax + playerSpeedModifier));
	else if (AEInputCheckCurr('D') || AEInputCheckCurr(VK_RIGHT) && hitSide != 3) vel.X(playerSpeedMax + playerSpeedModifier);
	else vel.X(0);

	PlatformManager::Platform *platform = PlatformManager::IsOnPlatform(&behavior.parent, &newPos);
	//If a jump button is pressed state that we are jumping and no longer grounded.
	if (AEInputCheckTriggered('W') || AEInputCheckTriggered(VK_SPACE) || AEInputCheckTriggered(VK_UP)) {
		if (platform) {
			vel.Y(playerJumpSpeedMax+platform->jump);
			playerSpeedModifier += 15.0f;
		}
	}

	//If we are in the air, apply gravity.
	if(inAir) {
		vel.Y(vel.Y() - playerGravity * dt);
	}

	//If we just landed on a platform.
	if (platform && inAir) {
		inAir = false;
		vel.Y(0);
	}

	if (platform) {
		if (platform->dir != Vector2D(0, 0)) {
			//newPos += platform->dir * platform->moveSpeed * dt;
			newPos.X(newPos.X() + platform->dir.X() * platform->moveSpeed * dt);
		}
		behavior.parent.GetTransform()->SetTranslation(newPos);
	}

	if(!platform) inAir = true;

	if (vel.X() > playerSpeedMax) vel.X(playerSpeedMax);
	if (vel.X() < -playerSpeedMax) vel.X(-playerSpeedMax);

	p->SetVelocity(vel);

	/*Vector2D newVel = Vector2D();
	if (PlatformManager::HittingSide(&behavior.parent, &newPos, &newVel)) {
		behavior.parent.GetTransform()->SetTranslation(newPos);
		p->SetVelocity(newVel);
	} else {
		p->SetVelocity(vel);
	}*/

	// If we are touching a platform and were last frame, incrment a counter.
	if (wasGrounded && platform)
	{
		touchTime += dt;

		if (touchTime > maxPlayerTime)
		{
			//BehaviorCheckpoint::ResetUnconditional();
		}
	}
	else
	{
		touchTime = 0;
	}

	wasGrounded = platform;
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


