//------------------------------------------------------------------------------
//
// File Name:	GameStatePlatformLevel1.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
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
#include "GameStatePlatformLevel1.h"
#include "Trace.h"
#include "GameObject.h"
#include "LevelTimer.h"
#include "BehaviorCheckpoint.h"
#include "PlatformManager.h"
#include "BehaviorPlayer.h"
#include "BehaviorGoal.h"
#include "Mesh.h"
#include "Transform.h"
#include "SpriteSource.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

AEGfxTexture* GameStatePlatformLevel1::texturePlayer;
AEGfxVertexList* GameStatePlatformLevel1::meshQuad;
SpriteSource* GameStatePlatformLevel1::spriteSourcePlayer;

AEGfxTexture* GameStatePlatformLevel1::textureGoal;
AEGfxVertexList* GameStatePlatformLevel1::meshGoal;
SpriteSource* GameStatePlatformLevel1::spriteSourceGoal;

AEGfxTexture* GameStatePlatformLevel1::textureCheckpoint;
AEGfxVertexList* GameStatePlatformLevel1::meshCheckpoint;
SpriteSource* GameStatePlatformLevel1::spriteSourceCheckpoint;

LevelTimer* GameStatePlatformLevel1::timer;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the GameStatePlatformLevel1 game state.
void GameStatePlatformLevel1::Load()
{
	Trace::GetInstance().GetStream() << "GameStatePlatformLevel1: Load" << std::endl;

	meshQuad = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh3x3");
	texturePlayer = AEGfxTextureLoad("Assets\\player.png");
	spriteSourcePlayer = new SpriteSource(1, 1, texturePlayer);

	meshCheckpoint = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	textureCheckpoint = AEGfxTextureLoad("Assets\\player.png");
	spriteSourceCheckpoint = new SpriteSource(3, 3, textureCheckpoint);

	meshGoal = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	textureGoal = AEGfxTextureLoad("Assets\\goal.png");
	spriteSourceGoal = new SpriteSource(4, 1, textureGoal);
}

// Initialize the memory associated with the GameStatePlatformLevel1 game state.
void GameStatePlatformLevel1::Init()
{
	Trace::GetInstance().GetStream() << "GameStatePlatformLevel1: Init" << std::endl;

	AEGfxSetBackgroundColor(1, 1, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	PlatformManager::Init();

	timer = new LevelTimer({ -300, 200 }, { 10, 10 });

	CreatePlayer({-500, -100});
	CreateGoal({ 2300, 600 }, GameStateTable::GsLevel1);

	//Creating map.
	Transform pt = Transform(-500, -150);
	pt.SetScale({ 100, 20 });

	PlatformManager::AddPlatform(pt);
	pt.SetTranslation({ -250, -150 });
	PlatformManager::AddPlatform(pt);
	pt.SetTranslation({ 0, -150 });
	PlatformManager::AddPlatform(pt);
	pt.SetTranslation({ 250, -150 });
	PlatformManager::AddPlatform(pt, 500);
	pt.SetTranslation({ 300, 100 });
	pt.SetScale({ 20, 500 });
	PlatformManager::AddPlatform(pt);
	pt.SetTranslation({ 450, 300 });
	pt.SetScale({ 200, 20 });
	PlatformManager::AddPlatform(pt);
	pt.SetScale({ 100, 20 });
	pt.SetTranslation({ 800, 100 });
	PlatformManager::AddPlatform(pt, 50, true);
	CreateCheckpoint({ 1100, 500 });
	pt.SetTranslation({ 1300, 450 });
	PlatformManager::AddPlatform(pt, 0, false, 250, { {1100, 700}, {1500, 1000}, {1500, 450} }, false);
	pt.SetTranslation({ 1700, 550 });
	PlatformManager::AddPlatform(pt, 0, false, 250, { {1700, 1100}, {2300, 1400}, {2300, 800}, {2000, 800} }, false);
}

// Update the GameStatePlatformLevel1 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStatePlatformLevel1::Update(float dt)
{
	Trace::GetInstance().GetStream() << "GameStatePlatformLevel1: Update" << std::endl;

	timer->Update(dt);
	PlatformManager::Update(dt);

	if (AEInputCheckCurr('R'))
	{
		BehaviorCheckpoint::ResetUnconditional();
	}
}

void GameStatePlatformLevel1::UpdateAO(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	Transform::SetCamTranslation(-GameObjectManager::GetInstance().GetObjectByName("Player")->GetTransform()->GetTranslation());
}

// Shutdown any memory associated with the GameStatePlatformLevel1 game state.
void GameStatePlatformLevel1::Shutdown()
{
	Trace::GetInstance().GetStream() << "GameStatePlatformLevel1: Shutdown" << std::endl;

	PlatformManager::Shutdown();
	GameObjectManager::GetInstance().Shutdown();
	AEGfxMeshFree(meshQuad);
	AEGfxTextureUnload(texturePlayer);
	AEGfxMeshFree(meshGoal);
	AEGfxTextureUnload(textureGoal);
	AEGfxMeshFree(meshCheckpoint);
	AEGfxTextureUnload(textureCheckpoint);
}

// Unload the resources associated with the GameStatePlatformLevel1 game state.
void GameStatePlatformLevel1::Unload()
{
	Trace::GetInstance().GetStream() << "GameStatePlatformLevel1: Unload" << std::endl;
}

void GameStatePlatformLevel1::CreatePlayer(Vector2D loc)
{
	CreateCheckpoint(loc);

	GameObject* gameObjectPlayer = new GameObject("Player");

	Transform* transform = new Transform(loc.X(), loc.Y());
	transform->SetScale(Vector2D(100, 100));

	Sprite* sprite2 = new Sprite("Player Sprite");
	sprite2->SetMesh(meshQuad);
	sprite2->SetSpriteSource(spriteSourcePlayer);

	Animation* animation2 = new Animation(sprite2);
	animation2->Play(8, 0.25f, true);

	Physics* physics = new Physics();

	gameObjectPlayer->SetAnimation(*animation2);
	gameObjectPlayer->SetPhysics(*physics);
	gameObjectPlayer->SetSprite(*sprite2);
	gameObjectPlayer->SetTransform(*transform);

	Collider* col = new Collider(*gameObjectPlayer);
	gameObjectPlayer->SetCollider(*col);

	Behavior* behavior = (Behavior*)new BehaviorPlayer(*gameObjectPlayer);
	gameObjectPlayer->SetBehavior(*behavior);

	BehaviorCheckpoint::SetPlayer(*gameObjectPlayer);

	GameObjectManager::GetInstance().Add(*gameObjectPlayer);
}

void GameStatePlatformLevel1::CreateGoal(Vector2D loc, int targetLevel)
{
	GameObject* gameObjectGoal = new GameObject("Goal");

	Transform* transform = new Transform(loc.X(), loc.Y());
	transform->SetScale({ 100, 100 });

	Sprite* sprite2 = new Sprite("Goal Sprite");
	sprite2->SetMesh(meshGoal);
	sprite2->SetSpriteSource(spriteSourceGoal);

	Animation* animation2 = new Animation(sprite2);
	animation2->Play(4, 0.25f, true);

	Physics* physics = new Physics();

	gameObjectGoal->SetAnimation(*animation2);
	gameObjectGoal->SetPhysics(*physics);
	gameObjectGoal->SetSprite(*sprite2);
	gameObjectGoal->SetTransform(*transform);

	Collider* col = new Collider(*gameObjectGoal);
	gameObjectGoal->SetCollider(*col);

	Behavior* behavior = (Behavior*)new BehaviorGoal(*gameObjectGoal, targetLevel);
	gameObjectGoal->SetBehavior(*behavior);

	GameObjectManager::GetInstance().Add(*gameObjectGoal);

	Transform pTransform = Transform(loc.X(), loc.Y() - 50);
	pTransform.SetScale({ 150, 20 });
	PlatformManager::AddPlatform(pTransform, 0, false, 0, {}, false);
}

void GameStatePlatformLevel1::CreateCheckpoint(Vector2D loc)
{
	GameObject* gameObjectCheckpoint = new GameObject("Checkpoint");

	Transform* transform = new Transform(loc.X(), loc.Y());
	transform->SetScale(Vector2D(100, 100));

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

	GameObjectManager::GetInstance().Add(*gameObjectCheckpoint);

	Transform pTransform = Transform(loc.X(), loc.Y() - 50);
	pTransform.SetScale({ 150, 20 });
	PlatformManager::AddPlatform(pTransform, 0, false, 0, {}, false);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

