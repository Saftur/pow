#include "stdafx.h"
#include "BehaviorSpaceship.h"
#include <AEEngine.h>
#include "GameObjectManager.h"
#include "GameStateManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"
#include "Physics.h"
#include "Collider.h"
#include "Random.h"
#include "Teleporter.h"

enum ShipState
{
	cSpaceshipIdle,
	cSpaceshipThrust,
	cSpaceshipDead,
};

BehaviorSpaceship::BehaviorSpaceship()
{
	SetNextState(cSpaceshipIdle);
}

Component * BehaviorSpaceship::Clone() const
{
	return new BehaviorSpaceship(*this);
}

void BehaviorSpaceship::OnEnter()
{
	GameObject *parent_ = GetParent();
	switch (GetCurrentState()) {
	case cSpaceshipIdle:
	{
		if (parent_) {
			Collider * collider = (Collider*)parent_->GetComponent("Collider");
			if (collider)
				collider->SetCollisionHandler(CollisionHandler);
		}
		break;
	}
	case cSpaceshipDead:
	{
		SetTimer(spaceshipDeathDuration);
		if (parent_) {
			Physics* physics = (Physics*)parent_->GetComponent("Physics");
			float rotationalVelocity = physics->GetRotationalVelocity();

			if (rotationalVelocity < 0.0f)
				rotationalVelocity = RandomRangeFloat(3.0f, 6.0f);
			else
				rotationalVelocity = -RandomRangeFloat(3.0f, 6.0f);

			physics->SetRotationalVelocity(rotationalVelocity);
		}
		break;
	}
	}
}

void BehaviorSpaceship::OnUpdate(float dt)
{
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	TeleporterUpdateObject(*parent_);
	switch (GetCurrentState()) {
	case cSpaceshipIdle:
		UpdateRotation();
		UpdateWeapon(dt);
		if (AEInputCheckCurr(VK_UP))
			SetNextState(cSpaceshipThrust);
		break;
	case cSpaceshipThrust:
		UpdateRotation();
		UpdateVelocity(dt);
		UpdateWeapon(dt);
		if (!AEInputCheckCurr(VK_UP))
			SetNextState(cSpaceshipIdle);
		break;
	case cSpaceshipDead:
		Sprite* sprite = (Sprite*)parent_->GetComponent("Sprite");
		int time = (int)(GetTimer() * 3.0f) % 2;
		switch (time)
		{
		case 0:
			sprite->AdjustAlpha(-8.0f * dt);
			break;
		case 1:
			sprite->AdjustAlpha(8.0f * dt);
			break;
		default:
			break;
		}
		SetTimer(GetTimer() - dt);
		if (GetTimer() <= 0)
			GameStateManager::GetInstance().SetNextState(GameStateTable::GsRestart);
	}
}

void BehaviorSpaceship::UpdateRotation()
{
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	Physics *physics = (Physics*)parent_->GetComponent("Physics");
	if (!physics) return;
	if (AEInputCheckCurr(VK_LEFT))
		physics->SetRotationalVelocity(spaceshipTurnRateMax);
	else if (AEInputCheckCurr(VK_RIGHT))
		physics->SetRotationalVelocity(-spaceshipTurnRateMax);
	else physics->SetRotationalVelocity(0);
}

void BehaviorSpaceship::UpdateVelocity(float dt)
{
	GameObject *parent_ = GetParent();
	Transform *transform = (Transform*)parent_->GetComponent("Transform");
	Physics *physics = (Physics*)parent_->GetComponent("Physics");
	if (!transform || !physics) return;

	float rotation = transform->GetRotation();
	Vector2D dir = Vector2D::FromAngleRadians(rotation);
	Vector2D velocity = physics->GetVelocity();

	velocity += dir * spaceshipAcceleration * dt;
	float speed = velocity.Magnitude();
	if (speed > spaceshipSpeedMax)
		velocity *= (spaceshipSpeedMax / speed);

	physics->SetVelocity(velocity);
}

void BehaviorSpaceship::UpdateWeapon(float dt)
{
	if (GetTimer() > 0) {
		SetTimer(GetTimer() - dt);
		if (GetTimer() < 0)
			SetTimer(0);
	}

	if (GetTimer() <= 0 && AEInputCheckCurr(' ')) {
		SpawnBullet();
		SetTimer(spaceshipWeaponCooldownTime);
	}
}

void BehaviorSpaceship::SpawnBullet()
{
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	GameObject *bulletArchetype = GameObjectManager::GetInstance().GetArchetype("Bullet");
	Transform *transform = (Transform*)parent_->GetComponent("Transform");
	if (!bulletArchetype || !transform) return;

	GameObject *bullet = new GameObject(*bulletArchetype);
	Transform *bulletTransform = (Transform*)bullet->GetComponent("Transform");
	Physics *bulletPhysics = (Physics*)bullet->GetComponent("Physics");
	if (!bulletTransform || !bulletPhysics) {
		delete bullet;
		return;
	}

	Vector2D position = transform->GetTranslation();
	float rotation = transform->GetRotation();
	bulletTransform->SetTranslation(position);
	bulletTransform->SetRotation(rotation);
	Vector2D dir = Vector2D::FromAngleRadians(rotation);
	bulletPhysics->SetVelocity(dir * spaceshipWeaponBulletSpeed);

	GameObjectManager::GetInstance().Add(*bullet);
}

void BehaviorSpaceship::CollisionHandler(GameObject & spaceship, GameObject & other)
{
	if (other.IsNamed("Asteroid")) {
		Behavior * behavior = (Behavior*)spaceship.GetComponent("Behavior");
		if (behavior)
			behavior->SetNextState(cSpaceshipDead);
	}
}
