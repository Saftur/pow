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
#include "Transform.h"
#include <vector>
using std::vector;

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

/**
 * @param t1  Transform of first box
 * @param t2  Transform of second box
 * @param pps (Contact) points per side (of box)
 * @return List of contact points
 * @warning Transform::rotation is not currently taken into account
 */
vector<Vector2D> BoxBoxIntersection(const Transform& t1, const Transform& t2, int pps);

/**
 * @param point          Point to check
 * @param boxTranslation Translation of box to check (bottom-left corner)
 * @param boxScale       Scale of box to check
 */
bool PointBoxIntersection(const Vector2D& point, const Vector2D& boxTranslation, const Vector2D& boxScale);