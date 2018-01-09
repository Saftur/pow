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
#include "GameObjectManager.h"
#include "GameStateMenu.h"
#include "GameObject.h"
#include "Trace.h"
#include "Mesh.h"
#include "Transform.h"
#include "SpriteSource.h"

#include "BehaviorLevelButton.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateMenu::Load()
{
	Trace::GetInstance().GetStream() << "Menu: Load" << std::endl;
}

// Initialize the memory associated with the Stub game state.
void GameStateMenu::Init()
{
	Trace::GetInstance().GetStream() << "Menu: Init" << std::endl;

	AEGfxSetBackgroundColor(1, 1, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	GameObject* button = new GameObject("LoadLevel1Button");

	Transform* transform = new Transform(0, 0);
	transform->SetScale(Vector2D(75, 37));
	button->SetTransform(*transform);

	AEGfxVertexList* meshQuad = MeshCreateQuad(1.0f, 1.0f, 1.0f, 1.0f, "Mesh3x3");
	AEGfxTexture* texture = AEGfxTextureLoad("Assets\\LoadLevel1Button.png");
	SpriteSource* spritesource = new SpriteSource(1, 1, texture);

	Sprite* sprite = new Sprite("LoadLevel1Sprite");
	sprite->SetMesh(meshQuad);
	sprite->SetSpriteSource(spritesource);
	button->SetSprite(*sprite);

	Collider* collider = new Collider(*button);
	button->SetCollider(*collider);

	Behavior* behavior = (Behavior*)new BehaviorLevelButton(*button);
	((BehaviorLevelButton*)behavior)->SetTargetLevel(GameStateTable::GsLevel1);
	button->SetBehavior(*behavior);

	GameObjectManager::GetInstance().Add(*button);

	GameObject* button2 = new GameObject("LoadLevel2Button");

	Transform* transform2 = new Transform(0, -100.0f);
	transform2->SetScale(Vector2D(75, 37));
	button2->SetTransform(*transform2);

	AEGfxTexture* texture2 = AEGfxTextureLoad("Assets\\LoadLevel2Button.png");
	SpriteSource* spritesource2 = new SpriteSource(1, 1, texture2);

	Sprite* sprite2 = new Sprite("LoadLevel2Sprite");
	sprite2->SetMesh(meshQuad);
	sprite2->SetSpriteSource(spritesource2);
	button2->SetSprite(*sprite2);

	Collider* collider2 = new Collider(*button2);
	button2->SetCollider(*collider2);

	Behavior* behavior2 = (Behavior*)new BehaviorLevelButton(*button2);
	((BehaviorLevelButton*)behavior2)->SetTargetLevel(GameStateTable::GsLevel2);
	button2->SetBehavior(*behavior2);

	GameObjectManager::GetInstance().Add(*button2);

	GameObject* rando = new GameObject("thingy");
	Transform* randoTransform = new Transform(100000, 100000);
	rando->SetTransform(*randoTransform);
	Collider* randoCollider = new Collider(*rando);
	rando->SetCollider(*randoCollider);
	GameObjectManager::GetInstance().Add(*rando);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateMenu::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	if (AEInputCheckCurr('1'))
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsLevel1);
	if (AEInputCheckCurr('2'))
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsLevel2);
	if (AEInputCheckCurr('3'))
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsAsteroids);

	Trace::GetInstance().GetStream() << "Menu: Update" << std::endl;
}

// Shutdown any memory associated with the Stub game state.
void GameStateMenu::Shutdown()
{
	Trace::GetInstance().GetStream() << "Menu: Shutdown" << std::endl;

	GameObjectManager::GetInstance().Shutdown();
}

// Unload the resources associated with the Stub game state.
void GameStateMenu::Unload()
{
	Trace::GetInstance().GetStream() << "Menu: Unload" << std::endl;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

