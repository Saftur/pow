#include "stdafx.h"
#include "Collider.h"

Collider::Collider(ColliderType type) :
		Component("Collider"), type(type), handler(nullptr)
{
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

void Collider::Load(const rapidjson::Value& obj)
{

}
