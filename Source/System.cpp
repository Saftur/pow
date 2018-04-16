//------------------------------------------------------------------------------
//
// File Name:	System.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "System.h"
#include "Engine.h"
#include "Gamepad.h"

//------------------------------------------------------------------------------
// Libraries
//------------------------------------------------------------------------------

#ifdef _DEBUG
#pragma comment (lib, "Alpha_Engine_D.lib")
#else
#pragma comment (lib, "Alpha_Engine.lib")
#endif

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Store the Windows settings passed in WinMain.
void System::StoreWindowSettings(HINSTANCE instanceH, int show)
{
	systemInfo.mAppInstance = instanceH;
	systemInfo.mCreateWindow = show != 0;
}

// Initialize the System (Windows, Event Handlers).
void System::Init()
{
	// Initialize the system 
	AESysInitInfo sysInitInfo;
	sysInitInfo.mAppInstance = systemInfo.mAppInstance;
	sysInitInfo.mShow = systemInfo.mCreateWindow;
	sysInitInfo.mWinWidth = 800;
	sysInitInfo.mWinHeight = 600;
	sysInitInfo.mCreateConsole = 1;
	sysInitInfo.mMaxFrameRate = 60;
	sysInitInfo.mpWinCallBack = NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
	sysInitInfo.mWindowStyle = WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;		
	sysInitInfo.mCreateWindow = 1;
	sysInitInfo.mWindowHandle = NULL;
	AESysInit(&sysInitInfo);

	// Change the window title.
	AESysSetWindowTitle("CS230 Project 2 - Graphics and Animation");

	// reset the system modules
	AESysReset();
}

// Update the System (Windows, Event Handlers).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void System::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	// Informing the system about the loop's start
	AESysFrameStart();

	// Handling Input
	AEInputUpdate();
	Gamepad::UpdateAll();

	// check if the window doesn't exist.
	if (!AESysDoesWindowExist())
	{
		//LevelManager::GetInstance().Quit();
		Engine::GetInstance().Quit();
	}
}

// Complete the draw process for the current game loop.
void System::Draw()
{
	// Informing the system about the loop's end
	AESysFrameEnd();
}

// Shutdown the System (Windows, Event Handlers).
void System::Shutdown()
{
	// Shutdown the Alpha Engine.
	AESysExit();
}

// Retrieve the instance of the System singleton.
System & System::GetInstance()
{
	static System instance;
	return instance;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation.
System::System()
{
}
