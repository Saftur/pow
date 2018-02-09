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

vector<Vector2D> BoxBoxIntersection(const Transform & t1, const Transform & t2, int pps)
{
	vector<Vector2D> contact;
	Vector2D scl1 = t1.GetScale();
	Vector2D scl2 = t2.GetScale();
	Vector2D trs1 = t1.GetTranslation() - scl1/2; // start from left/bottom point of box (less math later)
	Vector2D trs2 = t2.GetTranslation() - scl2/2;
	Vector2D point;
	int dx, dy;

	for (int i = 0; i < pps; i++) {
		dx = i * (scl1.x / (pps + 1));
		dy = i * (scl1.y / (pps + 1));
		point = {trs1.x + dx, trs1.y};
		if (PointBoxIntersection(point, trs2, scl2))
			contact.push_back(point);
		point = {trs1.x + dx, trs1.y + scl1.y};
		if (PointBoxIntersection(point, trs2, scl2))
			contact.push_back(point);
		point = {trs1.x, trs1.y + dy};
		if (PointBoxIntersection(point, trs2, scl2))
			contact.push_back(point);
		point = {trs1.x + scl1.x, trs1.y + dy};
		if (PointBoxIntersection(point, trs2, scl2))
			contact.push_back(point);

		dx = i * (scl2.x / (pps + 1));
		dy = i * (scl2.y / (pps + 1));
		point = {trs2.x + dx, trs2.y};
		if (PointBoxIntersection(point, trs1, scl1))
			contact.push_back(point);
		point = {trs2.x + dx, trs2.y + scl2.y};
		if (PointBoxIntersection(point, trs1, scl1))
			contact.push_back(point);
		point = {trs2.x, trs2.y + dy};
		if (PointBoxIntersection(point, trs1, scl1))
			contact.push_back(point);
		point = {trs2.x + scl2.x, trs2.y + dy};
		if (PointBoxIntersection(point, trs1, scl1))
			contact.push_back(point);
	}
	
	return contact;
}

bool PointBoxIntersection(const Vector2D & point, const Vector2D & boxTranslation, const Vector2D & boxScale)
{
	return point.x >= boxTranslation.x && point.y < boxTranslation.x + boxScale.x &&
		   point.y >= boxTranslation.y && point.y < boxTranslation.y + boxScale.y;
}
