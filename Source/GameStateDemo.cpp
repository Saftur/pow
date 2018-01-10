//------------------------------------------------------------------------------
//
// File Name:	GameStateDemo.c
// Author(s):	Arthur Bouvier
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateDemo.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Demo game state.
void GameStateDemo::Load()
{
	Trace::GetInstance().GetStream() << "Demo: Load" << std::endl;
}

// Initialize the memory associated with the Demo game state.
void GameStateDemo::Init()
{
	Trace::GetInstance().GetStream() << "Demo: Init" << std::endl;
}

// Update the Demo game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateDemo::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "Demo: Update" << std::endl;

	//GameStateManager::GetInstance().SetNextState(GameStateTable::GsQuit);
}

// Shutdown any memory associated with the Demo game state.
void GameStateDemo::Shutdown()
{
	Trace::GetInstance().GetStream() << "Demo: Shutdown" << std::endl;
}

// Unload the resources associated with the Demo game state.
void GameStateDemo::Unload()
{
	Trace::GetInstance().GetStream() << "Demo: Unload" << std::endl;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

