#include "stdafx.h"
#include "Intersection2D.h"

bool CircleCircleIntersection(const Vector2D & center1, float radius1, const Vector2D & center2, float radius2)
{
	float distX = center1.X() - center2.X();
	float distY = center1.Y() - center2.Y();
	float dist = sqrt(distX * distX + distY * distY);

	if (dist <= radius1 + radius2) {
		return true;
	}
	
	return false;
}
