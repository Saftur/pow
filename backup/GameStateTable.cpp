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

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

const int GameStateTable::GsQuit = -3;
const int GameStateTable::GsRestart = -2;
const int GameStateTable::GsInvalid = -1;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Determine if the game state is valid.
bool GameStateTable::StateIsValid(int gameState) const
{
	return ((0 <= gameState) && (gameState < GsNum) && GameStateTab[gameState] != nullptr);
}

// Determine if the game state is a "special" game state.
bool GameStateTable::StateIsSpecial(int gameState) const
{
	return ((gameState == GsRestart) || (gameState == GsQuit));
}

// Execute the Load function for the current game state.
void GameStateTable::ExecuteLoad(int gameState)
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState))
	{
		// Execute the Init function.
		GameStateTab[gameState]->Load();
	}
}

// Execute the Init function for the current game state.
void GameStateTable::ExecuteInit(int gameState)
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState))
	{
		// Execute the Init function.
		GameStateTab[gameState]->Init();
	}
}

// Execute the Update function for the current game state.
void GameStateTable::ExecuteUpdate(int gameState, float dt)
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState))
	{
		// Execute the Update function.
		GameStateTab[gameState]->Update(dt);
	}
}

// Execute the Shutdown function for the current game state.
void GameStateTable::ExecuteShutdown(int gameState)
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState))
	{
		// Execute the Shutdown function.
		GameStateTab[gameState]->Shutdown();
	}
}

// Execute the Unload function for the current game state.
void GameStateTable::ExecuteUnload(int gameState)
{
	// First validate the game state and the function pointer.
	if (StateIsValid(gameState))
	{
		// Execute the Init function.
		GameStateTab[gameState]->Unload();
	}
}

// Retrieve the id of a named game state.
int GameStateTable::GetStateId(const char * name) const
{
	for (unsigned i = 0; i < GsNum; ++i)
	{
		if (strcmp(GameStateTab[i]->GetName(), name) == 0)
			return i;
	}

	return GsInvalid;
}

void GameStateTable::Clear()
{
	for (unsigned i = 0; i < GsNum; ++i)
	{
		delete GameStateTab[i];
		GameStateTab[i] = nullptr;
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

// Destructor is private to prevent accidental destruction.
GameStateTable::~GameStateTable()
{
	Clear();
}
