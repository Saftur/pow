//------------------------------------------------------------------------------
//
// File Name:	Intersection2D.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Check whether two circles intersect.
// Params:
//  center1 = Center of the first circle.
//  radius1 = Radius of the first circle.
//  center2 = Center of the second circle.
//  radius2 = Radius of the second circle.
// Returns:
//   True if intersection, false otherwise.
bool CircleCircleIntersection(const Vector2D& center1, float radius1, const Vector2D& center2, float radius2);
