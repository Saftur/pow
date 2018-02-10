#include "stdafx.h"
#include "Collider.h"
#include "GameObject.h"

Collider::Collider(ColliderType type) :
		Component("Collider"), transform(0, 0), type(type), handler(nullptr)
{
	transform.SetParent((Transform*)GetParent()->GetComponent("Transform"));
}

void Collider::Draw() const
{
}

void Collider::CheckCollision(const Collider & other)
{
	if (IsCollidingWith(other)) {
		GameObject *go1 = GetParent(), *go2 = other.GetParent();
		if (!go1 || !go2) return;
		if (handler)
			handler(*go1, *go2);
		if (other.handler)
			other.handler(*go2, *go1);
	}
}

void Collider::SetCollisionHandler(CollisionEventHandler handler_)
{
	this->handler = handler_;
}

ColliderType Collider::GetType() const
{
	return type;
}

const Transform & Collider::GetTransform() const
{
	return transform;
}

void Collider::SetTransform(const Transform trs)
{
	transform.SetTranslation(trs.GetTranslation());
	transform.SetRotation(trs.GetRotation());
	transform.SetScale(trs.GetScale());
}
