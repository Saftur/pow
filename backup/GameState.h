//------------------------------------------------------------------------------
//
// File Name:	GameState.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface (functions) declared in the header.
class GameState
{
public:
	// Creates a game state.
	// Params:
	//   name = Name to use for this state.
	GameState(const char* name);

	// Destructor - virtual to ensure correct destructor is called.
	virtual ~GameState();

	// Load the resources associated with the game state.
	virtual void Load();

	// Initialize the memory associated with the game state.
	virtual void Init();

	// Update the game state.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	virtual void Update(float dt);

	// Shutdown any memory associated with the game state.
	virtual void Shutdown();

	// Unload the resources associated with the game state.
	virtual void Unload();

	// Returns the name of the state.
	const char* GetName() const;

private:
	// Name of the state, used when loading a specific state in the GSM.
	const char* name;
};

//------------------------------------------------------------------------------