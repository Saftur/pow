//------------------------------------------------------------------------------
//
// File Name:	Engine.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
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
#include "Space.h"
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
void Engine::Init(const char *startLevel)
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
	//GameObjectManager::GetInstance().Init();
	Button::ListEffects();

	// Initialize the game state manager.
	//LevelManager::GetInstance().Init(startLevel);
	LevelManager::StaticInit();
	Space::LoadLayer(0, startLevel, true, true);
	//LevelManager::LoadLayer(0, startLevel, true, true);

	hasPauseMenu = LevelManager::LevelExists("PauseLevel");

	//Initialize the pause menu.
	//PauseMenu::GetInstance().Init();

	//AddCamera({ 0, 0 }, { AEGfxGetWinMinX(), AEGfxGetWinMaxY() }, { AEGfxGetWinMaxX(), AEGfxGetWinMinY() }, { 0, 0 });
}

// Update the game engine.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Engine::Update(float dt)
{
	Trace::GetInstance().GetStream() << "Engine: Update" << std::endl;
	if (AEInputCheckTriggered(VK_ESCAPE)) TogglePaused();
	if (switchPause) {
		if (hasPauseMenu) {
			if (paused)
				Space::LoadLayer(3, "PauseLevel", false, true);
				//LevelManager::LoadLayer(3, "PauseLevel", false, true);
			else Space::DeleteLayer(3);//LevelManager::UnloadLayer(3);
		}
		switchPause = false;
	}

	// Update the System (Windows, Event Handlers).
	System::GetInstance().Update(dt);

	// Update the game state manager.
	//LevelManager::GetInstance().Update(dt);
	//LevelManager::UpdateAll(dt);

	// Update the game object manager.
	//if(!paused) GameObjectManager::GetInstance().Update(dt);
	//GameObjectManager::UpdateAll(dt);

	Space::UpdateAll(dt);

	// Check for collisions.
	//GameObjectManager::GetInstance().CheckCollisions();

	// Draw objects.
	/*for (Camera *c : cameras) {
		//Transform::SetCamTranslation(c->);
		GameObjectManager::DrawAll(c);
	}*/
	Space::DrawAll();

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
	//GameObjectManager::GetInstance().Shutdown();
	//GameObjectManager::ShutdownLayers();

	//PauseMenu::GetInstance().Shutdown(true);

	// Shutdown the game state manager.
	//LevelManager::GetInstance().Shutdown();
	//LevelManager::ShutdownLayers();

	Space::ShutdownLayers();

	//--------------------------------------------------------------------------
	// NOTE: Certain modules need to be shutdown last and in reverse order.
	//--------------------------------------------------------------------------

	// Shutdown the System (Windows, Event Handlers).
	System::GetInstance().Shutdown();

	// Shutdown the Tracing/Logging module.
	Trace::GetInstance().Shutdown();

	/*for (Camera *c : cameras)
		delete c;
	cameras.clear();*/
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
	paused = !paused;
	switchPause = true;
}

void Engine::SetPaused(bool pause)
{
	paused = pause;
	switchPause = true;
}

bool Engine::IsRunning()
{
	return running;
}

void Engine::Quit()
{
	running = false;
}

/*void Engine::AddCamera(Camera * cam) {
	if (std::find(cameras.begin(), cameras.end(), cam) == cameras.end())
		cameras.push_back(cam);
}*/

/*Camera * Engine::GetCurrCamera() const {
	if ((unsigned)camNum < cameras.size())
		return cameras[camNum];
	else return nullptr;
}*/

/*void Engine::DelCamera(Camera * cam) {
	vector<Camera*>::iterator found = std::find(cameras.begin(), cameras.end(), cam);
	if (found != cameras.end())
		cameras.erase(found);
}*/

/*Vector2D* Engine::AddCamera(Vector2D screenPos, Vector2D topLeft, Vector2D bottomRight, Vector2D worldPos)
{
	cameras.push_back({ screenPos, topLeft, bottomRight, worldPos });
	return &(cameras.at(cameras.size() - 1).worldPos);
}*/

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation
Engine::Engine()
{
}
