//------------------------------------------------------------------------------
//
// File Name:	Vector2D.h
// Author(s):	Jeremy Kings (j.kings)
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

#include "stdafx.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Frequently, vector and matrix structures are declared publicly.  While an
// interface is typically provided with these structures, these are two common
// exceptions to the coding best practice of "hiding information".

// WARNING: Do not modify the declaration of this structure, as it is a part
// of the public interface.

class Vector2D
{
public:
	//------------------------------------------------------------------------------
	// Public Functions
	//------------------------------------------------------------------------------

	// Constructors (2)
	Vector2D();
	Vector2D(float x, float y);

	// Accessors (2)
	float X() const;
	float Y() const;

	// Mutators (2)
	void X(float x);
	void Y(float y);

	// Other functions (7)
	float MagnitudeSquared() const;
	float Magnitude() const;
	Vector2D Normalized() const;
	float GetAngleDegrees() const;
	float GetAngleRadians() const;
	float DistanceSquared(const Vector2D& other) const;
	float Distance(const Vector2D& other) const;
	Vector2D Midpoint(const Vector2D& other) const;
	float DotProduct(const Vector2D& other) const;

	// Static functions (2)
	static Vector2D FromAngleDegrees(float angle);
	static Vector2D FromAngleRadians(float angle);

	// Overloaded operators (7 member functions)
	Vector2D operator+(const Vector2D& other) const;
	Vector2D operator-(const Vector2D& other) const;
	Vector2D operator*(const Vector2D& other) const;
	Vector2D operator/(const Vector2D& other) const;
	Vector2D operator*(float scalar) const;
	Vector2D operator/(float scalar) const;
	Vector2D operator-() const;
	Vector2D& operator+=(const Vector2D& other);
	Vector2D& operator-=(const Vector2D& other);
	Vector2D& operator*=(const Vector2D& other);
	Vector2D& operator/=(const Vector2D& other);
	Vector2D& operator*=(float scalar);
	Vector2D& operator/=(float scalar);

	bool operator==(const Vector2D& other) const;
	bool operator!=(const Vector2D& other) const;

	float x; // The x-coordinate of a Vector2D
	float y; // The y-coordinate of a Vector2D

private:
	//------------------------------------------------------------------------------
	// Private Data
	//------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
// Public Functions (non-member, non-friend)
//------------------------------------------------------------------------------

// Overloaded operators (2 non-member, non-friend function)
std::ostream& operator<<(std::ostream& os, const Vector2D& vector);
Vector2D operator*(float scalar, const Vector2D& vector);

//------------------------------------------------------------------------------
