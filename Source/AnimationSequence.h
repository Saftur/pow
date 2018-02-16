//------------------------------------------------------------------------------
//
// File Name:	AnimationSequence.h
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

#include "AnimationFrame.h"
#include <vector>
using std::vector;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this structure as long as you do not
//   change the public functions declared in the header.
class AnimationSequence
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------
	// Allocate a new animation sequence object.
	// Params:
	//	 frameCount = The number of frames in the sequence.
	//	 frameList = A pointer to an array of AnimationFrame records.
	//	 isLooping = True if the animation loops, false otherwise.
	AnimationSequence(vector<AnimationFrame> frameList, bool isLooping);

	AnimationSequence();

	// Get the specified frame information from the animation sequence.
	// Params:
	//	 sequenceIndex = Desired offset into the animation sequence list.
	// Returns:
	//	 If the animation sequence index is valid,
	//		then return the frame information at the specified offset,
	//		else return a pointer to an 'empty' animation frame { 0, 0.0f },
	const AnimationFrame* GetFrame(unsigned int sequenceIndex) const;

	// Get the maximum number of frames in the specified animation sequence.
	// Returns:
	//	 The value in frameCount.
	unsigned int GetFrameCount() const;

	// Determine if the animation sequence is set to loop.
	// Returns:
	//	 The value in isLooping.
	bool IsLooping() const;

private:
	// A pointer to an array of AnimationFrame records.
	vector<AnimationFrame> frameList;

	// True if the animation loops, false otherwise.
	bool isLooping;
};
/*----------------------------------------------------------------------------*/
