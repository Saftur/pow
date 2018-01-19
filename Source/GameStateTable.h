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
#include "GameStateLevel1.h"
#include "GameStateLevel2.h"
#include "GameStateAsteroids.h"
#include "GameStateDemo.h"

//------------------------------------------------------------------------------

class GameStateTable
{
public:
	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------
	typedef enum
	{
		/* Normal Game States: */
		// Add new game states here.
		GsLevel1,
		GsLevel2,
		GsAsteroids,
		GsDemo,

		/* Bookkeeping States: */
		GsNum,
		GsLast = GsNum - 1,

		/* Specify the initial game state */
		GsInitial = GsLevel1,
	} GameStates;

	//------------------------------------------------------------------------------
	// Public Consts:
	//------------------------------------------------------------------------------

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
	bool StateIsValid(GameStates gameState) const;

	// Determine if the game state is a "special" game state.
	bool StateIsSpecial(GameStates gameState) const;

	// Execute the Load function for the current game state.
	void ExecuteLoad(GameStates gameState) const;

	// Execute the Init function for the current game state.
	void ExecuteInit(GameStates gameState) const;

	// Execute the Update function for the current game state.
	void ExecuteUpdate(GameStates gameState, float dt) const;

	// Execute the Shutdown function for the current game state.
	void ExecuteShutdown(GameStates gameState) const;

	// Execute the Unload function for the current game state.
	void ExecuteUnload(GameStates gameState) const;

	// Retrieve the instance of the GameStateTable singleton.
	static GameStateTable& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Constructor is private to prevent accidental instantiation.
	GameStateTable();

	// Disable copy constructor and assignment operator
	GameStateTable(const GameStateTable&) = delete;
	GameStateTable& operator=(const GameStateTable&) = delete;

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------
	typedef void(*GameStateVoidFunctionPtr)();
	typedef void(*GameStateDtFunctionPtr)(float dt);

	struct TableEntry
	{
		GameStateVoidFunctionPtr	gameStateLoad;		/* Pointer to a game state load function. */
		GameStateVoidFunctionPtr	gameStateInit;		/* Pointer to a game state initialization function. */
		GameStateDtFunctionPtr		gameStateUpdate;	/* Pointer to a game state update function. */
		GameStateVoidFunctionPtr	gameStateShutdown;	/* Pointer to a game state shutdown function. */
		GameStateVoidFunctionPtr	gameStateUnload;	/* Pointer to a game state unload function. */
	};

	//------------------------------------------------------------------------------
	// Private Consts:
	//------------------------------------------------------------------------------
	const TableEntry GameStateTab[GsNum] =
	{
		{ GameStateLevel1::Load, GameStateLevel1::Init, GameStateLevel1::Update, GameStateLevel1::Shutdown, GameStateLevel1::Unload },
		{ GameStateLevel2::Load, GameStateLevel2::Init, GameStateLevel2::Update, GameStateLevel2::Shutdown, GameStateLevel2::Unload },
		{ GameStateAsteroids::Load, GameStateAsteroids::Init, GameStateAsteroids::Update, GameStateAsteroids::Shutdown, GameStateAsteroids::Unload },
		{ GameStateDemo::Load, GameStateDemo::Init, GameStateDemo::Update, GameStateDemo::Shutdown, GameStateDemo::Unload },
	};
};
//------------------------------------------------------------------------------
