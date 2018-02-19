//------------------------------------------------------------------------------
//
// File Name:	AnimationSequence.h
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S17
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
#include "AnimationFrame.h"

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

const AnimationFrame nullFrame = { 0, 0.0f };

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new animation sequence object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Params:
//	 frameCount = The number of frames in the sequence.
//	 frameList = A pointer to an array of AnimationFrame records.
//	 isLooping = True if the animation loops, false otherwise.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
AnimationSequence::AnimationSequence(vector<AnimationFrame> frameList, bool isLooping)
	: frameList(frameList), isLooping(isLooping) {}

AnimationSequence::AnimationSequence()
{
}

// Get the specified frame information from the animation sequence.
// Params:
//	 sequence = Pointer to the animation sequence object.
//	 sequenceIndex = Desired offset into the animation sequence list.
// Returns:
//	 If the animation sequence pointer is valid,
//		then return the frame information at the specified offset,
//		else return a pointer to an 'empty' animation frame { 0, 0.0f },
const AnimationFrame* AnimationSequence::GetFrame(unsigned int sequenceIndex) const
{
	// Return the sequence pointer's address + index * sizeof(AnimationFrame) addresses
	return &frameList[sequenceIndex];
}

// Get the maximum number of frames in the specified animation sequence.
// Params:
//	 sequence = Pointer to the animation sequence object.
// Returns:
//	 If the animation sequence pointer is valid,
//		then return the value in frameCount,
//		else return 0.
unsigned int AnimationSequence::GetFrameCount() const
{
	return frameList.size();
}

// Determine if the animation sequence is set to loop.
// Params:
//	 sequence = Pointer to the animation sequence object.
// Returns:
//	 If the animation sequence pointer is valid,
//		then return the value in isLooping,
//		else return false.
bool AnimationSequence::IsLooping() const
{
	return isLooping;
}

/*----------------------------------------------------------------------------*/



