#include "stdafx.h"
#include "GameStateAsteroids.h"
#include <AEEngine.h>
#include "GameStateManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"
#include "Physics.h"
#include "BehaviorSpaceship.h"
#include "BehaviorBullet.h"
#include "BehaviorAsteroid.h"
#include "ColliderCircle.h"
#include "Trace.h"
#include "LevelManager.h"

int GameStateAsteroids::asteroidScore = 0;
int GameStateAsteroids::asteroidHighScore = 0;
int GameStateAsteroids::asteroidWaveCount = 0;

char GameStateAsteroids::score[scoreStringLength];

GameStateAsteroids::GameStateAsteroids() :
		GameState("Asteroids")
{
}

void GameStateAsteroids::Load()
{
	Trace::GetInstance().GetStream() << "Asteroids: Load" << std::endl;

	LevelManager::GetInstance().Load("TestLevel");

	CreateMeshes();
}

void GameStateAsteroids::Init()
{
	Trace::GetInstance().GetStream() << "Asteroids: Init" << std::endl;

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

void GameStateAsteroids::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "Asteroids: Update" << std::endl;

	if (!GameObjectManager::GetInstance().GetObjectByName("Asteroid"))
		SpawnAsteroidWave();

	if (AEInputCheckTriggered('1'))
		GameStateManager::GetInstance().SetNextState("Level1");
	if (AEInputCheckTriggered('2'))
		GameStateManager::GetInstance().SetNextState("Level2");
	if (AEInputCheckTriggered('3'))
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsRestart);
}

void GameStateAsteroids::Shutdown()
{
	Trace::GetInstance().GetStream() << "Asteroids: Shutdown" << std::endl;

	GameObjectManager::GetInstance().Shutdown();
}

void GameStateAsteroids::Unload()
{
	Trace::GetInstance().GetStream() << "Asteroids: Unload" << std::endl;

	FreeMeshes();
}

void GameStateAsteroids::IncreaseScore(unsigned int scoreValue)
{
	asteroidScore += scoreValue;
	UpdateScore();
}

void GameStateAsteroids::CreateMeshes(void)
{
	// Create a “unit” - sized triangular mesh, as follows :
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	pMeshSpaceship = AEGfxMeshEnd();

	if (!pMeshSpaceship)
		Trace::GetInstance().GetStream() << "Failed to create spaceship mesh!" << std::endl;

	// Create a “unit” - sized triangular mesh, as follows :
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF808080, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFF0080, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFF808080, 0.0f, 0.0f);
	pMeshBullet = AEGfxMeshEnd();

	if (!pMeshBullet)
		Trace::GetInstance().GetStream() << "Failed to create bullet mesh!" << std::endl;

	// Create an asteroid mesh with a minimum of 5 sides.
	AEGfxMeshStart();
	// 4 sides for now, more later
	AEGfxTriAdd(
		-0.25f, -0.5f, 0xFF444444, 0.0f, 0.0f,
		0.25f, -0.5f, 0xFF444444, 0.0f, 0.0f,
		0.0f, 0.0f, 0xFF444444, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, 0.0f, 0xFF444444, 0.0f, 0.0f,
		0.25f, -0.5f, 0xFF444444, 0.0f, 0.0f,
		0.0f, 0.0f, 0xFF444444, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.5f, 0.0f, 0xFF444444, 0.0f, 0.0f,
		-0.25f, -0.5f, 0xFF444444, 0.0f, 0.0f,
		0.0f, 0.0f, 0xFF444444, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.25f, 0.5f, 0xFF444444, 0.0f, 0.0f,
		0.25f, 0.5f, 0xFF444444, 0.0f, 0.0f,
		0.0f, 0.0f, 0xFF444444, 0.0f, 0.0f);
	AEGfxTriAdd(
		-0.5f, 0.0f, 0xFF444444, 0.0f, 0.0f,
		-0.25f, 0.5f, 0xFF444444, 0.0f, 0.0f,
		0.0f, 0.0f, 0xFF444444, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, 0.0f, 0xFF444444, 0.0f, 0.0f,
		0.25f, 0.5f, 0xFF444444, 0.0f, 0.0f,
		0.0f, 0.0f, 0xFF444444, 0.0f, 0.0f);
	pMeshAsteroid = AEGfxMeshEnd();

	if(!pMeshAsteroid)
		Trace::GetInstance().GetStream() << "Failed to create asteroid mesh!" << std::endl;
}

void GameStateAsteroids::FreeMeshes(void)
{
	AEGfxMeshFree(pMeshSpaceship);
	AEGfxMeshFree(pMeshBullet);
	AEGfxMeshFree(pMeshAsteroid);
}

void GameStateAsteroids::CreateSpaceship(void)
{
	GameObject *spaceship = new GameObject("Spaceship");
	Transform *transform = new Transform(0, 0);
	transform->SetRotation(0);
	transform->SetScale({ 50, 40 });
	Sprite *sprite = new Sprite();
	sprite->SetMesh(pMeshSpaceship);
	Physics *physics = new Physics();
	Behavior *behavior = (Behavior*)(new BehaviorSpaceship());
	Collider *collider = new ColliderCircle(25);

	spaceship->AddComponent(transform);
	spaceship->AddComponent(sprite);
	spaceship->AddComponent(physics);
	spaceship->AddComponent(behavior);
	spaceship->AddComponent(collider);

	GameObjectManager::GetInstance().Add(*spaceship);
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
	GameObject *asteroid = GameObjectManager::GetInstance().GetArchetype("Asteroid");
	if (asteroid)
		GameObjectManager::GetInstance().Add(*(new GameObject(*asteroid)));
}

void GameStateAsteroids::UpdateScore(void)
{
	char title[100];
	sprintf(title, "CS230 Project 6 - Asteroids, Wave = %d, Score = %d, High Score = %d", asteroidWaveCount, asteroidScore, asteroidHighScore);
	AESysSetWindowTitle(title);
}

void GameStateAsteroids::CreateBulletArchetype(void)
{
	GameObject *bullet = new GameObject("Bullet");
	Transform *transform = new Transform(0, 0);
	transform->SetRotation(0);
	transform->SetScale({ 10, 10 });
	Sprite *sprite = new Sprite();
	sprite->SetMesh(pMeshBullet);
	Physics *physics = new Physics();
	Behavior *behavior = (Behavior*)(new BehaviorBullet());
	Collider *collider = new ColliderCircle(5);

	bullet->AddComponent(transform);
	bullet->AddComponent(sprite);
	bullet->AddComponent(physics);
	bullet->AddComponent(behavior);
	bullet->AddComponent(collider);

	GameObjectManager::GetInstance().AddArchetype(*bullet);
}

void GameStateAsteroids::CreateAsteroidArchetype(void)
{
	GameObject *asteroid = new GameObject("Asteroid");
	Transform *transform = new Transform(0, 0);
	transform->SetRotation(0);
	transform->SetScale({ 40, 40 });
	Sprite *sprite = new Sprite();
	sprite->SetMesh(pMeshAsteroid);
	Physics *physics = new Physics();
	Behavior *behavior = (Behavior*)(new BehaviorAsteroid());
	Collider *collider = new ColliderCircle(20);

	asteroid->AddComponent(transform);
	asteroid->AddComponent(sprite);
	asteroid->AddComponent(physics);
	asteroid->AddComponent(behavior);
	asteroid->AddComponent(collider);

	GameObjectManager::GetInstance().AddArchetype(*asteroid);
}
