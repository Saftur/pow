//------------------------------------------------------------------------------
//
// File Name:	GameStateTable.h
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

/* Include all game state headers here. */
#include "GameStateAsteroids.h"

//------------------------------------------------------------------------------

class GameStateTable
{
public:
	//------------------------------------------------------------------------------
	// Public Consts:
	//------------------------------------------------------------------------------

	// Starting state - currently set to first state in list
	static const int GsInitial = 0;
	// Number of states - set to the number of states you intend to use
	static const int GsNum = 3;

	// Quit state value
	static const int GsQuit;
	// Restart state value
	static const int GsRestart;
	// Invalid state value
	static const int GsInvalid;

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Determine if the game state is valid.
	bool StateIsValid(int gameState) const;

	// Determine if the game state is a "special" game state.
	bool StateIsSpecial(int gameState) const;

	// Execute the Load function for the current game state.
	void ExecuteLoad(int gameState);

	// Execute the Init function for the current game state.
	void ExecuteInit(int gameState);

	// Execute the Update function for the current game state.
	void ExecuteUpdate(int gameState, float dt);

	// Execute the Shutdown function for the current game state.
	void ExecuteShutdown(int gameState);

	// Execute the Unload function for the current game state.
	void ExecuteUnload(int gameState);

	// Retrieve the id of a named game state.
	int GetStateId(const char* name) const;

	// Clear out game state tab
	void Clear();

	// Retrieve the instance of the GameStateTable singleton.
	static GameStateTable& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Constructor is private to prevent accidental instantiation.
	GameStateTable();

	// Destructor is private to prevent accidental destruction.
	~GameStateTable();

	// Disable copy constructor and assignment operator
	GameStateTable(const GameStateTable&) = delete;
	GameStateTable& operator=(const GameStateTable&) = delete;

	// List of all game states
	GameState* GameStateTab[GsNum] =
	{
		new GameStateAsteroids(),
	};
};
//------------------------------------------------------------------------------
