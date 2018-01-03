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
}

GameObject* GameStateLevel1::CreateMonkey()
{
	gameObjectMonkey = new GameObject("Monkey");
	
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

	return gameObjectMonkey;
}

// Initialize the memory associated with the Stub game state.
void GameStateLevel1::Init()
{
	Trace::GetInstance().GetStream() << "Stub: Init" << std::endl;

	CreateMonkey();

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

	gameObjectMonkey->Update(dt);
	gameObjectMonkey->Draw();

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
}

// Shutdown any memory associated with the Stub game state.
void GameStateLevel1::Shutdown()
{
	Trace::GetInstance().GetStream() << "Stub: Shutdown" << std::endl;

	delete(gameObjectMonkey);
}

// Unload the resources associated with the Stub game state.
void GameStateLevel1::Unload()
{
	Trace::GetInstance().GetStream() << "Stub: Unload" << std::endl;

	AEGfxMeshFree(meshQuad);
	AEGfxTextureUnload(textureMonkey);

}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

