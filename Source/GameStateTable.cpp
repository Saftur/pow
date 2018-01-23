//------------------------------------------------------------------------------
//
// File Name:	GameStateTable.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateTable.h"

const int GameStateTable::GsQuit = -1;
const int GameStateTable::GsRestart = -2;
const int GameStateTable::GsInvalid = -3;


//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Determine if the game state is valid.
bool GameStateTable::StateIsValid(GameStates gameState) const
{
	return ((0 <= gameState) && (gameState < GsNum));
}

// Determine if the game state is a "special" game state.
bool GameStateTable::StateIsSpecial(GameStates gameState) const
{
	return ((gameState == GsRestart) || (gameState == GsQuit));
}

// Execute the Load function for the current game state.
void GameStateTable::ExecuteLoad(GameStates gameState) const
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateLoad) != NULL))
	{
		// Execute the Init function.
		(*GameStateTab[gameState].gameStateLoad)();
	}
}

// Execute the Init function for the current game state.
void GameStateTable::ExecuteInit(GameStates gameState) const
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateInit) != NULL))
	{
		// Execute the Init function.
		(*GameStateTab[gameState].gameStateInit)();
	}
}

// Execute the Update function for the current game state.
void GameStateTable::ExecuteUpdate(GameStates gameState, float dt) const
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateUpdate) != NULL))
	{
		// Execute the Update function.
		(*GameStateTab[gameState].gameStateUpdate)(dt);
	}
}

void GameStateTable::ExecuteUpdateAO(GameStates gameState, float dt) const
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateUpdateAO) != NULL))
	{
		// Execute the Update function.
		(*GameStateTab[gameState].gameStateUpdateAO)(dt);
	}
}

// Execute the Shutdown function for the current game state.
void GameStateTable::ExecuteShutdown(GameStates gameState) const
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateShutdown) != NULL))
	{
		// Execute the Shutdown function.
		(*GameStateTab[gameState].gameStateShutdown)();
	}
}

// Execute the Unload function for the current game state.
void GameStateTable::ExecuteUnload(GameStates gameState) const
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState) &&
		((*GameStateTab[gameState].gameStateUnload) != NULL))
	{
		// Execute the Init function.
		(*GameStateTab[gameState].gameStateUnload)();
	}
}

// Retrieve the instance of the GameStateTable singleton.
GameStateTable& GameStateTable::GetInstance()
{
	static GameStateTable instance;
	return instance;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation.
GameStateTable::GameStateTable()
{
}
