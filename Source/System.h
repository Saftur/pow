//------------------------------------------------------------------------------
//
// File Name:	System.h
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Windows.h>

//------------------------------------------------------------------------------

class System
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Store the Windows settings passed in WinMain.
	void StoreWindowSettings(HINSTANCE instanceH, int show);

	// Initialize the System (Windows, Event Handlers).
	void Init();

	// Update the System (Windows, Event Handlers).
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);

	// Complete the draw process for the current game loop.
	void Draw();

	// Shutdown the System (Windows, Event Handlers).
	void Shutdown();

	// Retrieve the instance of the System singleton.
	static System& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Constructor is private to prevent accidental instantiation.
	System();

	// Disable copy constructor and assignment operator
	System(const System&) = delete;
	System& operator=(const System&) = delete;

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------
	typedef struct SystemInfo
	{
		HINSTANCE mAppInstance;
		bool mCreateWindow;

	} SystemInfo;

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	SystemInfo systemInfo;
};
/*----------------------------------------------------------------------------*/
