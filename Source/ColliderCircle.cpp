#include "stdafx.h"
#include "ColliderCircle.h"
#include "GameObject.h"
#include "Transform.h"
#include "Intersection2D.h"

ColliderCircle::ColliderCircle(float radius) :
		Collider(ColliderTypeCircle), radius(radius)
{
}

Component * ColliderCircle::Clone() const
{
	return new ColliderCircle(*this);
}

void ColliderCircle::Draw(Camera *cam) const
{
}

float ColliderCircle::GetRadius() const
{
	return radius;
}

void ColliderCircle::SetRadius(float radius_)
{
	this->radius = radius_;
}

bool ColliderCircle::IsCollidingWith(const Collider & other) const
{
	GameObject *parent_ = GetParent(), *otherParent = other.GetParent();
	if (!parent_ || !otherParent) return false;
	Transform *tr1 = (Transform*)parent_->GetComponent("Transform");
	Transform *tr2 = (Transform*)otherParent->GetComponent("Transform");
	if (!tr1 || !tr2) return false;

	if (other.GetType() == ColliderTypeCircle) {
		return CircleCircleIntersection(tr1->GetTranslation(), radius, 
										tr2->GetTranslation(), ((ColliderCircle*)(&other))->radius);
	} else {
		return other.IsCollidingWith(*this);
	}
}

void ColliderCircle::Load(rapidjson::Value& obj)
{
	if (obj.HasMember("Radius"))
	{
		SetRadius(obj["Radius"].GetFloat());
	}
}
