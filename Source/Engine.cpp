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
#include "Space.h"
#include "AEEngine.h"
#include "Transform.h"
#include "Sprite.h"
#include "Button.h"
#include "SoundManager.h"

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
	SoundManager::GetInstance().Init();

	// Initialize the game state manager.
	LevelManager::StaticInit();
	Space::LoadLayer(0, startLevel, true, true);
}

// Update the game engine.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Engine::Update(float dt)
{
	Trace::GetInstance().GetStream() << "Engine: Update" << std::endl;

	// Update the System (Windows, Event Handlers).
	System::GetInstance().Update(dt);

	// Update all Spaces
	Space::UpdateAll(dt);

	// Draw all Spaces
	Space::DrawAll();

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

	// Shutdown Spaces
	Space::ShutdownLayers();

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

bool Engine::IsRunning()
{
	return running;
}

void Engine::Quit()
{
	running = false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation
Engine::Engine()
{
}
