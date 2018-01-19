//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.c
// Author(s):	Mark Culp
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateLevel1.h"
#include "GameObject.h"
#include "Trace.h"
#include "Mesh.h"
#include "Transform.h"
#include "SpriteSource.h"
#include "BehaviorCheckpoint.h"
#include "Collider.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
const float GameStateLevel1::monkeyWalkSpeed = 500.0f;
const float GameStateLevel1::monkeyJumpSpeed = 1000.0f;
const float GameStateLevel1::groundHeight = -150.0f;
const Vector2D GameStateLevel1::gravityNormal(0.0f, -1500.0f);
const Vector2D GameStateLevel1::gravityNone(0.0f, 0.0f);
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
AEGfxTexture* GameStateLevel1::textureMonkey;
AEGfxVertexList* GameStateLevel1::meshQuad;
GameObject* GameStateLevel1::gameObjectMonkey;
SpriteSource* GameStateLevel1::spriteSourceMonkey;

AEGfxTexture* GameStateLevel1::textureCheckpoint;
AEGfxVertexList* GameStateLevel1::meshCheckpoint;
GameObject* GameStateLevel1::gameObjectCheckpoint;
SpriteSource* GameStateLevel1::spriteSourceCheckpoint;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateLevel1::Load()
{
	Trace::GetInstance().GetStream() << "Stub: Load" << std::endl;

	meshQuad = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	textureMonkey = AEGfxTextureLoad("Assets\\MonkeyWalk.png");
	spriteSourceMonkey = new SpriteSource(3, 3, textureMonkey);

	meshCheckpoint = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	textureCheckpoint = AEGfxTextureLoad("Assets\\player.png");
	spriteSourceCheckpoint = new SpriteSource(3, 3, textureCheckpoint);
}

GameObject* GameStateLevel1::CreateMonkey()
{
	gameObjectMonkey = new GameObject("Player");

	Transform* transform = new Transform(0, groundHeight);
	transform->SetScale(Vector2D(300, 300));

	Sprite* sprite2 = new Sprite("Monkey Sprite");
	sprite2->SetMesh(meshQuad);
	sprite2->SetSpriteSource(spriteSourceMonkey);

	Animation* animation2 = new Animation(sprite2);
	animation2->Play(8, 0.25f, true);

	Physics* physics = new Physics();

	gameObjectMonkey->SetAnimation(*animation2);
	gameObjectMonkey->SetPhysics(*physics);
	gameObjectMonkey->SetSprite(*sprite2);
	gameObjectMonkey->SetTransform(*transform);

	Collider* col = new Collider(*gameObjectMonkey);
	gameObjectMonkey->SetCollider(*col);

	return gameObjectMonkey;
}

GameObject* GameStateLevel1::CreateCheckpoint()
{
	gameObjectCheckpoint = new GameObject("Checkpoint");

	Transform* transform = new Transform(300, groundHeight);
	transform->SetScale(Vector2D(300, 300));

	Sprite* sprite2 = new Sprite("Checkpoint Sprite");
	sprite2->SetMesh(meshQuad);
	sprite2->SetSpriteSource(spriteSourceCheckpoint);

	Animation* animation2 = new Animation(sprite2);
	animation2->Play(8, 0.25f, true);

	Behavior* bCh = (Behavior*)new BehaviorCheckpoint(*gameObjectCheckpoint);

	Physics* physics = new Physics();

	gameObjectCheckpoint->SetAnimation(*animation2);
	gameObjectCheckpoint->SetPhysics(*physics);
	gameObjectCheckpoint->SetSprite(*sprite2);
	gameObjectCheckpoint->SetTransform(*transform);
	gameObjectCheckpoint->SetBehavior(*bCh);

	Collider* col = new Collider(*gameObjectCheckpoint);
	gameObjectCheckpoint->SetCollider(*col);

	return gameObjectCheckpoint;
}

// Initialize the memory associated with the Stub game state.
void GameStateLevel1::Init()
{
	Trace::GetInstance().GetStream() << "Stub: Init" << std::endl;

	CreateMonkey();
	CreateCheckpoint();

	GameObjectManager::GetInstance().Add(*gameObjectMonkey);
	GameObjectManager::GetInstance().Add(*gameObjectCheckpoint);

	AEGfxSetBackgroundColor(1, 1, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

void GameStateLevel1::MoveMonkey()
{
	Physics* objPhysics = gameObjectMonkey->GetPhysics();
	Transform* objTransform = gameObjectMonkey->GetTransform();

	if (objPhysics == NULL || objTransform == NULL) return;

	Vector2D objVelocity = objPhysics->GetVelocity();

	if (AEInputCheckCurr(VK_LEFT))
	{
		objVelocity.X(-monkeyWalkSpeed);
	}
	else if (AEInputCheckCurr(VK_RIGHT))
	{
		objVelocity.X(monkeyWalkSpeed);
	}
	else
	{
		objVelocity.X(0);
	}

	if (AEInputCheckCurr(VK_UP))
	{
		objVelocity.Y(monkeyJumpSpeed);
		objPhysics->SetAcceleration(GameStateLevel1::gravityNormal);
	}

	if (objTransform->GetTranslation().Y() < groundHeight)
	{
		objTransform->SetTranslation(Vector2D(objTransform->GetTranslation().X(), groundHeight));
		objVelocity.Y(0);
		objPhysics->SetAcceleration(GameStateLevel1::gravityNone);
	}

	objPhysics->SetVelocity(objVelocity);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "Stub: Update" << std::endl;

	MoveMonkey();

	if (AEInputCheckCurr('1'))
	{
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsRestart);
	}
	else if (AEInputCheckCurr('2'))
	{
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsLevel2);
	}
	else if (AEInputCheckCurr('3'))
	{
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsAsteroids);
	}
	else if (AEInputCheckCurr('R'))
	{
		BehaviorCheckpoint::ResetUnconditional();
	}
}

// Shutdown any memory associated with the Stub game state.
void GameStateLevel1::Shutdown()
{
	Trace::GetInstance().GetStream() << "Stub: Shutdown" << std::endl;

	GameObjectManager::GetInstance().Shutdown();
}

// Unload the resources associated with the Stub game state.
void GameStateLevel1::Unload()
{
	Trace::GetInstance().GetStream() << "Stub: Unload" << std::endl;

	AEGfxMeshFree(meshQuad);
	AEGfxMeshFree(meshCheckpoint);
	AEGfxTextureUnload(textureMonkey);
	AEGfxTextureUnload(textureCheckpoint);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

