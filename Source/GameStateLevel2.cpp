//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel2.c
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
#include "GameStateLevel2.h"
#include "Trace.h"
#include "GameObject.h"
#include "LevelTimer.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
const float GameStateLevel2::spaceshipSpeed = 500.0f;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
int GameStateLevel2::numLives;
AEGfxVertexList* GameStateLevel2::meshTriangle;
GameObject* GameStateLevel2::gameObjectShip;

LevelTimer* GameStateLevel2::timer;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateLevel2::Load()
{
	Trace::GetInstance().GetStream() << "Stub: Load" << std::endl;

	FILE* file = fopen("data/Level2_Lives.txt", "rt");

	if (file != NULL)
	{
		fscanf(file, "%d", &numLives);
		fclose(file);
	}
	else
	{
		puts("Couldn't open Level2_Lives.txt!");
	}

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	meshTriangle = AEGfxMeshEnd();
}

// Creates the spaceship
GameObject* GameStateLevel2::CreateSpaceship()
{
	gameObjectShip = new GameObject("Dorito");

	Transform* transform = new Transform(0.0f, 0.0f);
	transform->SetScale(Vector2D(100, 100));

	Sprite* sprite = new Sprite("Dorito");
	sprite->SetMesh(meshTriangle);

	Physics* physics = new Physics();

	gameObjectShip->SetPhysics(*physics);
	gameObjectShip->SetSprite(*sprite);
	gameObjectShip->SetTransform(*transform);

	return gameObjectShip;
}

// Initialize the memory associated with the Stub game state.
void GameStateLevel2::Init()
{
	Trace::GetInstance().GetStream() << "Stub: Init" << std::endl;

	CreateSpaceship();
	timer = new LevelTimer({ -300, 100 }, { 10, 10 });

	AEGfxSetBackgroundColor(1, 1, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Moves the spaceship
void GameStateLevel2::MoveSpaceship()
{
	Transform* transform = gameObjectShip->GetTransform();
	Physics* physics = gameObjectShip->GetPhysics();

	if (transform == NULL || physics == NULL) return;

	s32 mouseX, mouseY;
	float sX, sY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &sX, &sY);

	Vector2D translation = transform->GetTranslation();

	Vector2D vector = Vector2D(sX - translation.X(), sY - translation.Y());
	vector = vector.Normalized();

	transform->SetRotation(atan2f(vector.Y(), vector.X()));

	physics->SetVelocity(vector * spaceshipSpeed);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel2::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */

	Trace::GetInstance().GetStream() << "Stub: Update" << std::endl;

	MoveSpaceship();
	gameObjectShip->Update(dt);
	gameObjectShip->Draw();
	timer->Update(dt);

	if (AEInputCheckCurr('1'))
	{
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsLevel1);
	}
	else if (AEInputCheckCurr('2'))
	{
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsRestart);
	}
	else if (AEInputCheckCurr('3'))
	{
		GameStateManager::GetInstance().SetNextState(GameStateTable::GsAsteroids);
	}
}

// Shutdown any memory associated with the Stub game state.
void GameStateLevel2::Shutdown()
{
	Trace::GetInstance().GetStream() << "Stub: Shutdown" << std::endl;

	delete(gameObjectShip);
}

// Unload the resources associated with the Stub game state.
void GameStateLevel2::Unload()
{
	Trace::GetInstance().GetStream() << "Stub: Unload" << std::endl;

	delete(meshTriangle);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

