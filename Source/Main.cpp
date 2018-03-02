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
#include "LevelManager.h"
#include "System.h"
#include <Windows.h>

//------------------------------------------------------------------------------
// Main Function:
//------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show){
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);

	// Change in time (in seconds) since the last game loop.
	// A framerate controller is not required for this assignment.
	float dt = 0.0167f;
	DWORD lastTime = 0;
	DWORD currentTime = GetTickCount();

	// Store the Windows settings passed in WinMain.
	System::GetInstance().StoreWindowSettings(instanceH, show);

	// Initialize the game engine.
	Engine::GetInstance().Init("DemoLevel");

	// MAIN LOOP: Do not exit until instructed to do so by the game state manager (GSM).
	while (/*LevelManager::GetInstance().IsRunning()*/Engine::GetInstance().IsRunning())
	{
		currentTime = GetTickCount();
		//if (lastTime > 0) dt = (float) (currentTime - lastTime)/1000;
		lastTime = currentTime;

		// Update the game engine.
		Engine::GetInstance().Update(AEInputCheckCurr(' ') ? dt * 10 : dt);
	}

	// Shutdown the game engine.
	Engine::GetInstance().Shutdown();

	_CrtDumpMemoryLeaks();

	return 0;
}
