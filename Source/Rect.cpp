#include "stdafx.h"
#include "Rect.h"

Rect::Rect(Vector2D pos, Vector2D size) {
	float halfX = size.x/2.f, halfY = size.y/2.f;
	top = pos.y+halfY;
	bottom = pos.y-halfY;
	left = pos.x-halfX;
	right = pos.x+halfX;
}

Vector2D Rect::Center() const {
	return BottomLeft().Midpoint(TopRight());
}

Vector2D Rect::Size() const {
	return TopRight() - BottomLeft();
}

Vector2D Rect::TopLeft() const {
	return {left, top};
}

Vector2D Rect::TopRight() const {
	return {right, top};
}

Vector2D Rect::BottomLeft() const {
	return {left, bottom};
}

Vector2D Rect::BottomRight() const {
	return {right, bottom};
}

bool Rect::IsInside(Rect rect) const {
	return rect.top < top && rect.bottom > bottom && rect.left > left && rect.right > right;
}

bool Rect::IsInside(Vector2D pos, Vector2D size) const {
	return IsInside(Rect(pos, size));
}
