/*******************************************************************************
Filename: Vector2D.c
Author: Mark Culp (mark.culp)
Course: CS170
Project: CS170 Project 3
Date: 25 October 2017
Description: Contains implementations of functions in Vector2D.h

******************************************************************************/
#include "stdafx.h"
#include "Vector2D.h"  // Vector2D members
#include <cmath>    // sqrt, atan, sin, cos

///////////////////////////////////////////////////////////////////////////////
// 22 public member functions (2 constructors, 2 accessors, 2 mutators, 7 others, 9 operators) 

// Constructors (2)
Vector2D::Vector2D(float x, float y)
	: x(x), y(y) {}
Vector2D::Vector2D()
	: x(0.0f), y(0.0f) {}

// Accessors (2)
// Returns this vector's X value.
// Params:
//	<none>
// Returns:
//	the x-value of the vector
float Vector2D::X() const
{
	return x;
}
	
// Returns this vector's Y value.
// Params:
//  <none>
// Returns:
//  the y-value of the vector
float Vector2D::Y() const
{
	return y;
}

// Mutators (2)
// Sets the X value of the vector.
// Params:
//  f - the value to set X to.
// Returns:
//  <void>
void Vector2D::X(float f)
{
	x = f;
}

// Sets the Y value of the vector.
// Params:
//  f - the value to set Y to.
// Returns:
//  <void>
void Vector2D::Y(float f)
{
	y = f;
}

// Other functions (7)
// Calculates the magnitude of the vector. (sqrt((x^2)+(y^2)))
// Params:
//  <none>
// Returns:
//  the magnitude of the vector.
float Vector2D::Magnitude() const
{
	return sqrtf((x * x) + (y * y));
}

// Squares the magnitude of the vector.
// Params:
//  <none>
// Returns:
//  the magnitude of the vector squared.
float Vector2D::MagnitudeSquared() const
{
	float mag = Magnitude();

	return (mag * mag);
}

// Normalizes the vector. ((1/magnitude)*x), (1/magnitude)*y))
// Params:
//  <none>
// Returns:
//  the normalized vector.
Vector2D Vector2D::Normalized() const
{
	float mag = Magnitude();

	return Vector2D(((1 / mag) * x), ((1 / mag) * y));
}

float Vector2D::GetAngleDegrees() const
{
	return GetAngleRadians() / (float)M_PI * 180.0f;
}

float Vector2D::GetAngleRadians() const
{
	return (x == 0 ? (float)M_PI / 2 : atanf(y/x)) + (y < 0 ? (float)M_PI : 0);
}

// Calculates the distance between two vectors.
// Params:
//  other - the other Vector2D.
// Returns:
//  the distance as a float.
float Vector2D::Distance(const Vector2D &other) const
{
	return sqrtf(((other.x - x) * (other.x - x)) + ((other.Y() - y) * (other.Y() - y)));
}

// Calculates the squared distance between two vectors.
// Params:
//  other - the other Vector2D.
// Returns:
//  the squared distance.
float Vector2D::DistanceSquared(const Vector2D &other) const
{
	return (Distance(other) * Distance(other));
}

// Calculates the midpoint between two vectors.
// Params:
//  other - the other Vector2D.
// Returns:
//  the midpoint.
Vector2D Vector2D::Midpoint(const Vector2D &other) const
{
	return Vector2D((other.x + x) / 2, (other.y + y) / 2);
}

// Calculates the dot product of two vectors.
// Params:
//  other - the other Vector2D.
// Returns:
//  the dot product as a float.
float Vector2D::DotProduct(const Vector2D &other) const
{
	return ((x * other.x) + (y * other.y));
}

// Static functions (2)
Vector2D Vector2D::FromAngleDegrees(float angle)
{
	return FromAngleRadians(angle * (float)M_PI / 180.0f);
}

Vector2D Vector2D::FromAngleRadians(float angle)
{
	return Vector2D(cos(angle), sin(angle));
}

// Operators (9)
// Overload for adding vectors.
// Params:
//  other - the other Vector2D.
// Returns:
//  the result of the operation.
Vector2D Vector2D::operator+ (const Vector2D &other) const
{
	return Vector2D(x + other.x, y + other.y);
}

// Overload for subtracting vectors.
// Params:
//  other - the other Vector2D.
// Returns:
//  the result of the operation.
Vector2D Vector2D::operator- (const Vector2D &other) const
{
	return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(const Vector2D & other) const
{
	return Vector2D(x * other.x, y * other.y);
}

Vector2D Vector2D::operator/(const Vector2D & other) const
{
	return Vector2D(x / other.x, y / other.y);
}

// Overload for scaling vectors.
// Params:
//  floaty - the scalar.
// Returns:
//  the result of the operation.
Vector2D Vector2D::operator* (float floaty) const
{
	return Vector2D(x * floaty, y * floaty);
}

// Overload for dividing vectors by floats.
// Params:
//  floaty - the float to divide by.
// Returns:
//  the result of the operation.
Vector2D Vector2D::operator/ (float floaty) const
{
	return Vector2D(x / floaty, y / floaty);
}

// Overload for scaling vectors.
// Params:
//  floaty - the scalar.
// Returns:
//  a reference to the modified Vector2D.
Vector2D& Vector2D::operator+= (const Vector2D &other)
{
	x += other.x;
	y += other.y;

	return *this;
}

// Overload for the -= operator.
// Params:
//  other - the other vector
// Returns:
//  a reference to the modified Vector2D.
Vector2D& Vector2D::operator-= (const Vector2D &other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D & other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

Vector2D& Vector2D::operator/=(const Vector2D & other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

// Overload for scaling vectors.
// Params:
//  floaty - the scalar.
// Returns:
//  a reference to the modified Vector2D.
Vector2D& Vector2D::operator*= (float floaty)
{
	x *= floaty;
	y *= floaty;

	return *this;
}
	
// Overload for dividing vectors by floats.
// Params:
//  floaty - the float to divide by.
// Returns:
//  the result of the operation.
Vector2D& Vector2D::operator/= (float floaty)
{
	x /= floaty;
	y /= floaty;

	return *this;
}

bool Vector2D::operator==(const Vector2D & other) const
{
	return x == other.x && y == other.y;
}

bool Vector2D::operator!=(const Vector2D & other) const
{
	return x != other.x || y != other.y;
}

// Negates the given vector
// Params:
//  <none>
// Returns:
//  the result of the operation.
Vector2D Vector2D::operator-() const
{
	return Vector2D(x * -1, y * -1);
}

///////////////////////////////////////////////////////////////////////////////
// 2 non-member, non-friend functions (operators)
// Global overload for multiplying floats by vectors.
// Params:
//  floaty - the float to multiply by.
//	someVectorOrWhatever - the vector or whatever.
// Returns:
//  the result of the operation.
Vector2D operator*(float floaty, const Vector2D someVectorOrWhatever)
{
	return someVectorOrWhatever * floaty;
}

// prints out a vector
std::ostream& operator<<(std::ostream &os, const Vector2D& vec)
{
	os << "(" << vec.X() << ", " << vec.Y() << ")";

	return os;
}

Vector2D operator*(float scalar, const Vector2D& vector)
{
	Vector2D v = Vector2D();
	v.X(vector.X() * scalar);
	v.Y(vector.Y() * scalar);

	return v;
}



