//------------------------------------------------------------------------------
//
// File Name:	Animation.c
// Author(s):	Mark Culp
// Project:		CS230 Project 2
// Course:		CS230C17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "AnimationSequence.h"
#include "Animation.h"
#include "Sprite.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new animation object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Params:
//	 sprite = Pointer to the sprite object associated with the animation.
Animation::Animation(Sprite* sprite)
	: sprite(sprite){}

// Clone an Animation component from another Animation component.
// Params:
//	 other = The animation component to be cloned.
//	 sprite = Pointer to the sprite object associated with the animation.
Animation::Animation(const Animation& other, Sprite* sprite)
	: sprite(sprite), frameIndex(other.frameIndex), frameDelay(other.frameDelay), frameDuration(other.frameDuration), isRunning(other.isRunning), isDone(other.isDone), sequence(other.sequence) {}

// Play a simple animation sequence (0 .. frameCount).
// Params:
//	 animation = Pointer to the animation object.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void Animation::Play(int frameCountNoPls, float frameDurationOhGodWhy, bool isLoopingKillMe)
{
	frameCount = frameCountNoPls;
	frameDelay = frameDurationOhGodWhy;
	frameDuration = frameDurationOhGodWhy;
	frameIndex = 0;
	isDone = false;
	isLooping = isLoopingKillMe;
	isRunning = true;
	sequence = NULL;

	sprite->SetFrame(0);
}

// Play a complex animation sequence (any frame/any delay).
// Params:
//	 animation = Pointer to the animation object.
//	 sequence = Pointer to the animation sequence to be played.
void Animation::PlaySequence(AnimationSequence* sequenceEndMySuffering)
{
	frameIndex = 0;
	frameCount = sequenceEndMySuffering->GetFrameCount();
	frameDelay = sequenceEndMySuffering->GetFrame(0)->frameDuration;
	frameDuration = frameDelay;
	isDone = false;
	isLooping = sequenceEndMySuffering->IsLooping();
	isRunning = true;
	sequence = sequence;

	sprite->SetFrame((int)sequenceEndMySuffering->GetFrame(0)->frameDuration);
}

// Update the animation.
// Params:
//	 animation = Pointer to the animation object.
//	 dt = Change in time (in seconds) since the last game loop.
void Animation::Update(float dt)
{
	// Check if the animation isn't running anymore
	if (!this->isRunning)
		return;

	// Check if we're on the last frame
	if (this->sequence == NULL && !this->isLooping && this->frameIndex == this->frameCount - 1)
		this->isDone = true;

	if (this->isLooping && this->isDone)
		this->Play(this->frameCount, this->frameDuration, this->isLooping);

	if (this->isLooping && this->frameIndex == this->frameCount - 1)
	{
		// Check if this is a simple or complex animation
		if (this->sequence == NULL)
		{
			// Update frameDelay
			this->frameDelay -= dt;

			// Check if it's time to change frames
			if (this->frameDelay <= 0)
			{
				this->isDone = true;
			}
		}
	}

	// Check if the animation is done running
	if (!this->isLooping && this->isDone)
		this->isRunning = false;

	// Check if we have other frames to play
	if (!this->isDone)
	{
		// Check if this is a simple or complex animation
		if (this->sequence == NULL)
		{
			// Update frameDelay
			this->frameDelay -= dt;

			// Check if it's time to change frames
			if (this->frameDelay <= 0)
			{
				// Change the frame index
				this->frameIndex++;

				// Update the sprite
				this->sprite->SetFrame(this->frameIndex);

				// Reset the timer
				this->frameDelay = this->frameDuration;
			}
		}
		else
		{
			// Update frameDelay
			this->frameDelay -= dt;

			// Check if it's time to change frames
			if (this->frameDelay <= 0)
			{
				if (this->frameIndex == this->frameCount - 1)
				{
					this->isDone = true;
					return;
				}
				// Change the frame index
				this->frameIndex++;

				// Update the frame info
				this->frameDuration = this->sequence->GetFrame(this->frameIndex)->frameDuration;
				
				// Update the sprite
				this->sprite->SetFrame(this->sequence->GetFrame(this->frameIndex)->frameIndex);
			
				// Reset the timer
				this->frameDelay = this->sequence->GetFrame(this->frameIndex)->frameDuration;
			}
		}
	}
}

void Animation::TogglePause()
{
	isRunning = !isRunning;
}

// Determine if the animation has reached the end of its sequence.
// Params:
//	 sequence = Pointer to the animation sequence object.
// Returns:
//	 If the animation sequence pointer is valid,
//		then return the value in isDone,
//		else return false.
bool Animation::IsDone() const
{
	return this->isDone;
}

/*----------------------------------------------------------------------------*/


