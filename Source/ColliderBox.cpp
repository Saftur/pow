#include "stdafx.h"
#include "ColliderBox.h"
#include "GameObject.h"

ColliderBox::ColliderBox() :
		Collider(ColliderTypeBox)
{
	transform.SetParent((Transform*)GetParent()->GetComponent("Transform"));
}

ColliderBox * ColliderBox::Clone() const
{
	return new ColliderBox(*this);
}

void ColliderBox::Draw() const
{
}

bool ColliderBox::IsCollidingWith(const Collider & other) const
{
	return false;
}
