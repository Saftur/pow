//------------------------------------------------------------------------------
//
// File Name:	AnimationFrame.h
// Author(s):	Doug Schilling (dschilling)
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// A public structure used for storing information on a single frame of animation.
// This structure is public, instead of private, because it is used for storing
// what is referred to as “plain old data” (POD).  In C++ programming, this is also
// referred to as a "C-Style Structure".
typedef struct AnimationFrame
{
	int		frameIndex;
	float	frameDuration;

} AnimationFrame;

//------------------------------------------------------------------------------

