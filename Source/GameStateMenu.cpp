//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.c
// Author(s):	Mark Culp
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateMenu.h"
#include "GameObject.h"
#include "Trace.h"
#include "Mesh.h"
#include "Transform.h"
#include "SpriteSource.h"

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
void GameStateMenu::Load()
{
	Trace::GetInstance().GetStream() << "Menu: Load" << std::endl;

}

// Initialize the memory associated with the Stub game state.
void GameStateMenu::Init()
{
	Trace::GetInstance().GetStream() << "Menu: Init" << std::endl;

	AEGfxSetBackgroundColor(1, 1, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateMenu::Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	Trace::GetInstance().GetStream() << "Menu: Update" << std::endl;
}

// Shutdown any memory associated with the Stub game state.
void GameStateMenu::Shutdown()
{
	Trace::GetInstance().GetStream() << "Menu: Shutdown" << std::endl;
}

// Unload the resources associated with the Stub game state.
void GameStateMenu::Unload()
{
	Trace::GetInstance().GetStream() << "Menu: Unload" << std::endl;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

