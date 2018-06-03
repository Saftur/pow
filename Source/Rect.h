#pragma once

#include "Vector2D.h"

struct Rect {
	Rect(Vector2D pos = {0.f, 0.f}, Vector2D size = {0.f, 0.f});

	float top;
	float bottom;
	float left;
	float right;

	Vector2D Center() const;
	Vector2D Size() const;

	Vector2D TopLeft() const;
	Vector2D TopRight() const;
	Vector2D BottomLeft() const;
	Vector2D BottomRight() const;

	bool IsInside(Rect rect) const;
	bool IsInside(Vector2D pos, Vector2D size = {0.f, 0.f}) const;
};
