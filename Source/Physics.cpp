#include "stdafx.h"
#include "Physics.h"
#include "GameObject.h"
#include "Transform.h"

Physics::Physics() :
		Component("Physics"), oldTranslation(0,0), acceleration(0,0), velocity(0,0), rotationalVelocity(0), inverseMass(0)
{
}

Component * Physics::Clone() const
{
	return new Physics(*this);
}

const Vector2D & Physics::GetAcceleration() const
{
	return acceleration;
}

const Vector2D & Physics::GetVelocity() const
{
	return velocity;
}

const Vector2D & Physics::GetOldTranslation() const
{
	return oldTranslation;
}

float Physics::GetRotationalVelocity() const
{
	return rotationalVelocity;
}

void Physics::SetAcceleration(const Vector2D & acceleration_)
{
	this->acceleration = acceleration_;
}

void Physics::SetVelocity(const Vector2D & velocity_)
{
	this->velocity = velocity_;
}

void Physics::SetRotationalVelocity(float velocity_)
{
	this->rotationalVelocity = velocity_;
}

void Physics::Update(float dt)
{
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	Transform *transform = parent_->GetComponent<Transform>();
	if (!transform) return;
	oldTranslation = transform->GetTranslation();
	velocity += acceleration * dt;
	transform->SetTranslation(oldTranslation + velocity * dt);
	transform->SetRotation(transform->GetRotation() + rotationalVelocity * dt);
}

void Physics::Load(rapidjson::Value& obj)
{
	UNREFERENCED_PARAMETER(obj);
}
