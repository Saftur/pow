//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateStub.h"
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

// Load the resources associated with the Stub game state.
void GameStateStub::Load()
{
	Trace::GetInstance().GetStream() << "Stub: Load" << std::endl;
}

// Initialize the memory associated with the Stub game state.
void GameStateStub::Init()
{
	Trace::GetInstance().GetStream() << "Stub: Init" << std::endl;
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateStub::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "Stub: Update" << std::endl;

	GameStateManager::GetInstance().SetNextState(GameStateTable::GsQuit);
}

// Shutdown any memory associated with the Stub game state.
void GameStateStub::Shutdown()
{
	Trace::GetInstance().GetStream() << "Stub: Shutdown" << std::endl;
}

// Unload the resources associated with the Stub game state.
void GameStateStub::Unload()
{
	Trace::GetInstance().GetStream() << "Stub: Unload" << std::endl;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

