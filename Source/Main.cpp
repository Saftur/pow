//------------------------------------------------------------------------------
//
// File Name:	Stub.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <windows.h>
#include "Engine.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "System.h"

//------------------------------------------------------------------------------
// Main Function:
//------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show){
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);

	// Change in time (in seconds) since the last game loop.
	// A framerate controller is not required for this assignment.
	float dt = 0.0167f;

	// Store the Windows settings passed in WinMain.
	System::GetInstance().StoreWindowSettings(instanceH, show);

	// Initialize the game engine.
	Engine::GetInstance().Init();

	// MAIN LOOP: Do not exit until instructed to do so by the game state manager (GSM).
	while (GameStateManager::GetInstance().IsRunning())
	{
		// Update the game engine.
		Engine::GetInstance().Update(dt);
		if (AEInputCheckCurr('0')) GameStateManager::GetInstance().SetNextState(GameStateTable::GsMenu);
	}

	// Shutdown the game engine.
	Engine::GetInstance().Shutdown();

	_CrtDumpMemoryLeaks();

	return 0;
}
