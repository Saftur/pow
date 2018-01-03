//------------------------------------------------------------------------------
//
// File Name:	GameStateAsteroids.cpp
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
#include "GameStateManager.h"
#include "GameObjectManager.h"
#include "GameStateAsteroids.h"
#include "BehaviorBullet.h"
#include "BehaviorSpaceship.h"
#include "BehaviorAsteroid.h"
#include "Random.h"
#include <string>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

AEGfxVertexList* GameStateAsteroids::pMeshSpaceship = NULL;
AEGfxVertexList* GameStateAsteroids::pMeshBullet = NULL;
AEGfxVertexList* GameStateAsteroids::pMeshAsteroid = NULL;
int GameStateAsteroids::asteroidScore = 0;
int GameStateAsteroids::asteroidHighScore = 0;
int GameStateAsteroids::asteroidSpawnCount;
int GameStateAsteroids::asteroidWaveCount;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Asteroids game state.
void GameStateAsteroids::Load()
{
	CreateMeshes();
}

// Initialize the memory associated with the Asteroids game state.
void GameStateAsteroids::Init()
{
	CreateSpaceship();
	CreateBulletArchetype();
	CreateAsteroidArchetype();

	if (asteroidScore > asteroidHighScore)
		asteroidHighScore = asteroidScore;

	asteroidScore = 0;
	asteroidWaveCount = 0;
	asteroidSpawnCount = cAsteroidSpawnInitial;

	SpawnAsteroidWave();

	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Asteroids game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateAsteroids::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (AEInputCheckCurr('1'))
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsLevel1);
	if (AEInputCheckCurr('2'))
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsLevel2);
	if (AEInputCheckCurr('3'))
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsRestart);

	if (!GameObjectManager::GetInstance().GetObjectByName("Asteroid"))
		SpawnAsteroidWave();
}

// Shutdown any memory associated with the Asteroids game state.
void GameStateAsteroids::Shutdown()
{
	GameObjectManager::GetInstance().Shutdown();
}

// Unload the resources associated with the Asteroids game state.
void GameStateAsteroids::Unload()
{
	FreeMeshes();
}

// Increase the asteroids score by score value.
// Params:
//	 scoreValue = The amount to be added to the game score.
void GameStateAsteroids::IncreaseScore(unsigned int scoreValue)
{
	asteroidScore += scoreValue;
	UpdateScore();
}

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

// Create meshes for spaceship and bullet
void GameStateAsteroids::CreateMeshes(void)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	pMeshSpaceship = AEGfxMeshEnd();

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF808080, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFF0080, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFF808080, 0.0f, 0.0f);
	pMeshBullet = AEGfxMeshEnd();

	AEGfxMeshStart();
	AEGfxTriAdd(-0.5f, 0.25f, 0x704F4680, 0.0f, 0.0f, 0.0f, 0.5f, 0x704F4680, 0.0f, 0.0f, 0.0f, 0.0f, 0x704F4680, 0.0f, 0.0f);
	AEGfxTriAdd(0.0f, 0.5f, 0x704F4680, 0.0f, 0.0f, 0.5f, 0.25f, 0x704F4680, 0.0f, 0.0f, 0.0f, 0.0f, 0x704F4680, 0.0f, 0.0f);
	AEGfxTriAdd(0.5f, 0.25f, 0x704F4680, 0.0f, 0.0f, 0.5f, -0.25f, 0x704F4680, 0.0f, 0.0f, 0.0f, 0.0f, 0x704F4680, 0.0f, 0.0f);
	AEGfxTriAdd(0.5f, -0.25f, 0x704F4680, 0.0f, 0.0f, 0.0f, -0.5f, 0x704F4680, 0.0f, 0.0f, 0.0f, 0.0f, 0x704F4680, 0.0f, 0.0f);
	AEGfxTriAdd(0.0f, -0.5f, 0x704F4680, 0.0f, 0.0f, -0.5f, -0.25f, 0x704F4680, 0.0f, 0.0f, 0.0f, 0.0f, 0x704F4680, 0.0f, 0.0f);
	AEGfxTriAdd(-0.5f, -0.25f, 0x704F4680, 0.0f, 0.0f, -0.5f, 0.25f, 0x704F4680, 0.0f, 0.0f, 0.0f, 0.0f, 0x704F4680, 0.0f, 0.0f);
	pMeshAsteroid = AEGfxMeshEnd();
}

// Free all created meshes.
void GameStateAsteroids::FreeMeshes(void)
{
	AEGfxMeshFree(pMeshSpaceship);
	AEGfxMeshFree(pMeshBullet);
	AEGfxMeshFree(pMeshAsteroid);
}

void GameStateAsteroids::CreateSpaceship(void)
{
	GameObject* go = new GameObject("Spaceship");

	Transform* t = new Transform(100, 0);
	t->SetScale(Vector2D(50, 40));
	go->SetTransform(*t);

	Sprite* sprite = new Sprite("Spaceship Sprite");
	sprite->SetMesh(pMeshSpaceship);

	Collider* c = new Collider(*go);

	Physics* p = new Physics();

	Behavior* b = (Behavior*)new BehaviorSpaceship(*go);

	go->SetSprite(*sprite);
	go->SetPhysics(*p);
	go->SetBehavior(*b);
	go->SetCollider(*c);

	GameObjectManager::GetInstance().Add(*go);
}

void GameStateAsteroids::SpawnAsteroidWave(void)
{
	asteroidWaveCount++;
	UpdateScore();
	
	for (int i = 0; i < asteroidSpawnCount; i++)
		SpawnAsteroid();

	if (asteroidSpawnCount < cAsteroidSpawnMaximum)
		asteroidSpawnCount++;
}

void GameStateAsteroids::SpawnAsteroid(void)
{
	GameObject* newObj = new GameObject(*GameObjectManager::GetInstance().GetArchetype("Asteroid"));
	GameObjectManager::GetInstance().Add(*newObj);
}

void GameStateAsteroids::UpdateScore(void)
{
	std::string title = "CS230 Project 6 - Asteroids, Wave = ";
	title += std::to_string(asteroidWaveCount);
	title += ", Score = ";
	title += std::to_string(asteroidScore);
	title += ", High Score = ";
	title += std::to_string(asteroidHighScore);
	AESysSetWindowTitle(title.c_str());
}

void GameStateAsteroids::CreateBulletArchetype(void)
{
	GameObject* go = new GameObject("Bullet");

	Transform* t = new Transform(0, 0);
	t->SetScale(Vector2D(10, 10));
	go->SetTransform(*t);

	Sprite* sprite = new Sprite("Bullet Sprite");
	sprite->SetMesh(pMeshBullet);

	Physics* p = new Physics();

	Collider* c = new Collider(*go);

	Behavior* b = (Behavior*) new BehaviorBullet(*go);

	go->SetSprite(*sprite);
	go->SetPhysics(*p);
	go->SetBehavior(*b);
	go->SetCollider(*c);

	GameObjectManager::GetInstance().AddArchetype(*go);
}

void GameStateAsteroids::CreateAsteroidArchetype(void)
{
	GameObject* go = new GameObject("Asteroid");

	Transform* t = new Transform(0, 0);
	t->SetScale(Vector2D(40, 40));
	go->SetTransform(*t);
	Sprite* s = new Sprite("Asteroid Sprite");
	s->SetMesh(pMeshAsteroid);

	Physics* p = new Physics();
	p->SetRotationalVelocity((float)M_PI / 4.0f);

	Collider* c = new Collider(*go);

	Behavior* b = (Behavior*) new BehaviorAsteroid(*go);

	go->SetBehavior(*b);
	go->SetCollider(*c);
	go->SetPhysics(*p);
	go->SetSprite(*s);

	GameObjectManager::GetInstance().AddArchetype(*go);
}

/*----------------------------------------------------------------------------*/
