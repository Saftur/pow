//------------------------------------------------------------------------------
//
// File Name:	Engine.h
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include <vector>
using std::vector;
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

class Engine
{
public:
	//------------------------------------------------------------------------------
	// Public Consts:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Initialize the game engine.
	void Init(const char *startLevel);

	// Update the game engine.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);

	// Shutdown the game engine.
	void Shutdown();

	// Retrieve the instance of the Engine singleton
	static Engine& GetInstance();

	bool IsRunning();

	void Quit();

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	// Constructor is private to prevent accidental instantiation
	Engine();

	// Disable copy constructor and assignment operator
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	bool running = true;
};
/*----------------------------------------------------------------------------*/
