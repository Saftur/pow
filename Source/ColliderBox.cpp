#include "stdafx.h"
#include "ColliderBox.h"

ColliderBox::ColliderBox() :
		Collider(ColliderTypeBox)
{
}

ColliderBox::ColliderBox(Transform transform_) :
		Collider(ColliderTypeBox)
{
	SetTransform(transform_);
}

ColliderBox * ColliderBox::Clone() const
{
	return new ColliderBox(*this);
}

void ColliderBox::Draw(Camera *cam) const
{
}

bool ColliderBox::IsCollidingWith(const Collider & other) const
{
	UNREFERENCED_PARAMETER(other);
	return false;
}
