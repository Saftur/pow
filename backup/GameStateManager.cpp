//------------------------------------------------------------------------------
//
// File Name:	GameStateManager.c
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
#include "Trace.h"
#include "GameObjectManager.h" // Shutdown

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game state manager.
void GameStateManager::Init()
{
	Trace::GetInstance().GetStream() << "GSM: Init" << std::endl;

	// Set the previous and current game states to invalid.
	gameStateIndices.previous = GameStateTable::GsInvalid;
	gameStateIndices.current = GameStateTable::GsInvalid;

	// Set the initial game state to launch the game.
	gameStateIndices.next = GameStateTable::GsInitial;
}

// Update the game state manager.
void GameStateManager::Update(float dt)
{
	Trace::GetInstance().GetStream() << "GSM: Update" << std::endl;

	// Check for a game state change.
	if (StateIsChanging())
	{
		// Shutdown the current game state.
		GameStateTable::GetInstance().ExecuteShutdown(gameStateIndices.current);
		// Free game objects
		GameObjectManager::GetInstance().Shutdown();

		// Handle unloading the current game state.
		if (gameStateIndices.next != GameStateTable::GsRestart)
		{
			GameStateTable::GetInstance().ExecuteUnload(gameStateIndices.current);
		}

		// Update the recorded states.
		gameStateIndices.previous = gameStateIndices.current;
		gameStateIndices.current = gameStateIndices.next;

		// Handle loading the new game state.
		if (gameStateIndices.current == GameStateTable::GsRestart)
		{
			// If we are restarting, current and next states should be the same as the previous
			gameStateIndices.current = gameStateIndices.previous;
			gameStateIndices.next = gameStateIndices.previous;
		}
		else
		{
			// Otherwise, load the next state
			GameStateTable::GetInstance().ExecuteLoad(gameStateIndices.current);
		}

		// Initialize the new game state.
		GameStateTable::GetInstance().ExecuteInit(gameStateIndices.current);
	}

	// Update the current game state.
	GameStateTable::GetInstance().ExecuteUpdate(gameStateIndices.current, dt);
}

// Shutdown the game state manager.
void GameStateManager::Shutdown()
{
	Trace::GetInstance().GetStream() << "GSM: Shutdown" << std::endl;

	// Delete all game states.
	GameStateTable::GetInstance().Clear();
}

// Determine if the current game state is being restarted.
bool GameStateManager::IsRestarting() const
{
	return gameStateIndices.next == GameStateTable::GsRestart;
}

// Determine if the game is still running.
bool GameStateManager::IsRunning() const
{
	return gameStateIndices.current != GameStateTable::GsQuit;
}

// Set the next game state to run.
void GameStateManager::SetNextState(int nextState)
{
	if (GameStateTable::GetInstance().StateIsValid(nextState) 
		|| GameStateTable::GetInstance().StateIsSpecial(nextState))
	{
		gameStateIndices.next = nextState;
	}
	else
	{
#ifdef _DEBUG
		/* In debug mode, print a message that an error has occurred. */
		Trace::GetInstance().GetStream() << "ERROR: Invalid game state: " << nextState << std::endl;
#endif
	}
}

// Set the next game state to run.
void GameStateManager::SetNextState(const char * name)
{
	int nextState = GameStateTable::GetInstance().GetStateId(name);
	SetNextState(nextState);
}

// Retrieve the instance of the GameStateManager singleton.
GameStateManager& GameStateManager::GetInstance()
{
	static GameStateManager instance;
	return instance;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

// Determine if a game state change has been requested.
bool GameStateManager::StateIsChanging() const
{
	return (gameStateIndices.current != gameStateIndices.next);
}

// Constructor is private to prevent accidental instantiations.
GameStateManager::GameStateManager()
{
}
