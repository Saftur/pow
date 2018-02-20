//------------------------------------------------------------------------------
//
// File Name:	Engine.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Engine.h"
#include "LevelManager.h"
#include "System.h"
#include "Trace.h"
#include "GameObjectManager.h"
#include "Random.h"
#include "LevelManager.h"
#include "AEEngine.h"
//#include "PauseMenu.h"
#include "Transform.h"
#include "Sprite.h"
#include "Button.h"

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game engine.
void Engine::Init()
{
	//--------------------------------------------------------------------------
	// NOTE: Certain modules need to be initialized first.
	//--------------------------------------------------------------------------

	// Initialize random number generator
	RandomInit();

	// Initialize the Tracing/Logging module.
	Trace::GetInstance().Init();

	Trace::GetInstance().GetStream() << "Engine: Init" << std::endl;

	// Initialize the System (Windows, Event Handlers).
	System::GetInstance().Init();

	//--------------------------------------------------------------------------
	// NOTE: Other modules can be initialized later and in any order.
	//--------------------------------------------------------------------------

	// Initialize the game object manager.
	GameObjectManager::GetInstance().Init();
	Button::ListEffects();

	// Initialize the game state manager.
	LevelManager::GetInstance().Init("DemoLevel");

	hasPauseMenu = LevelManager::LevelExists("PauseLevel");

	//Initialize the pause menu.
	//PauseMenu::GetInstance().Init();

	AddCamera({ 0, 0 }, { AEGfxGetWinMinX(), AEGfxGetWinMaxY() }, { AEGfxGetWinMaxX(), AEGfxGetWinMinY() }, { 0, 0 });
}

// Update the game engine.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Engine::Update(float dt)
{
	Trace::GetInstance().GetStream() << "Engine: Update" << std::endl;
	if (AEInputCheckTriggered(VK_ESCAPE)) TogglePaused();
	if (switchLevel) {
		if (paused)
			LevelManager::LoadAbove("PauseLevel", false, true);
		else LevelManager::UnloadAbove();
		switchLevel = false;
	}

	// Update the System (Windows, Event Handlers).
	System::GetInstance().Update(dt);

	// Update the game state manager.
	LevelManager::GetInstance().Update(dt);

	// Update the game object manager.
	//if(!paused) GameObjectManager::GetInstance().Update(dt);
	GameObjectManager::UpdateAll(dt);

	// Check for collisions.
	//GameObjectManager::GetInstance().CheckCollisions();

	// Draw objects.
	for (Camera c : cameras) {
		Transform::SetCamTranslation(c.worldPos);
		Sprite::SetBounds(c.topLeft, c.bottomRight);
		//GameObjectManager::GetInstance().Draw();
		GameObjectManager::DrawAll();
	}

	//PauseMenu::GetInstance().Update(dt);

	// Complete the draw process for the current game loop.
	System::GetInstance().Draw();
}

// Shutdown the game engine.
void Engine::Shutdown()
{
	Trace::GetInstance().GetStream() << "Engine: Shutdown" << std::endl;

	//--------------------------------------------------------------------------
	// NOTE: Some modules can be shutdown in any order.
	//--------------------------------------------------------------------------

	// Shutdown the game object manager.
	GameObjectManager::GetInstance().Shutdown();
	GameObjectManager::ShutdownInstances();

	//PauseMenu::GetInstance().Shutdown(true);

	// Shutdown the game state manager.
	LevelManager::GetInstance().Shutdown();
	LevelManager::ShutdownInstances();

	//--------------------------------------------------------------------------
	// NOTE: Certain modules need to be shutdown last and in reverse order.
	//--------------------------------------------------------------------------

	// Shutdown the System (Windows, Event Handlers).
	System::GetInstance().Shutdown();

	// Shutdown the Tracing/Logging module.
	Trace::GetInstance().Shutdown();
}

// Retrieve the instance of the Engine singleton
Engine& Engine::GetInstance()
{
	static Engine instance;
	return instance;
}

bool Engine::IsPaused() {
	return paused;
}

void Engine::TogglePaused() {
	if (!hasPauseMenu) return;
	paused = !paused;
	switchLevel = true;
}

Vector2D* Engine::AddCamera(Vector2D screenPos, Vector2D topLeft, Vector2D bottomRight, Vector2D worldPos)
{
	cameras.push_back({ screenPos, topLeft, bottomRight, worldPos });
	return &(cameras.at(cameras.size() - 1).worldPos);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation
Engine::Engine()
{
}
