//------------------------------------------------------------------------------
//
// File Name:	Animation.h
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

#include "AnimationSequence.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Sprite Sprite;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this class as long as you do not
// change the public functions declared in the header.
class Animation
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------
	// Allocate a new animation object.
	// Params:
	//	 sprite = Pointer to the sprite object associated with the animation.
	Animation(Sprite* sprite);

	// Disable copy constructor
	Animation(const Animation& other) = delete;

	// Clone an Animation component from another Animation component.
	// Params:
	//	 other = The animation component to be cloned.
	//	 sprite = Pointer to the sprite object associated with the animation.
	Animation(const Animation& other, Sprite* sprite);

	// Play a simple animation sequence (0 .. frameCount).
	// Params:
	//	 frameCount = The number of frames in the sequence.
	//	 frameDuration = The amount of time to wait between frames (in seconds).
	//	 isLooping = True if the animation loops, false otherwise.
	void Play(int frameCount, float frameDuration, bool isLooping);

	// Play a complex animation sequence (any frame/any delay).
	// Params:
	//	 sequence = Pointer to the animation sequence to be played.
	void PlaySequence(AnimationSequence* sequence);

	// Update the animation.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);

	// Determine if the animation has reached the end of its sequence.
	// Returns:
	//	 The value in isDone.
	bool IsDone() const;

	// Pauses/unpauses the current animation.
	void TogglePause();

private:
	// Pointer to the sprite associated with the animation.
	Sprite* sprite;

	// The current frame being displayed.
	unsigned int frameIndex;

	// The maximum number of frames in the sequence.
	unsigned int frameCount;

	// The time remaining for the current frame.
	float frameDelay;

	// The amount of time to display each frame.
	// (Used only when playing simple animations (0 .. numFrame).)
	float frameDuration;

	// True if the animation is running; false if the animation has stopped.
	bool isRunning;

	// True if the animation loops back to the beginning.
	bool isLooping;

	// True if the end of the animation has been reached, false otherwise.
	// (Hint: This should be true for only one game loop.)
	bool isDone;

	// The animation sequence currently being played, if any.
	AnimationSequence*	sequence;
};
/*----------------------------------------------------------------------------*/
