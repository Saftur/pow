#include "stdafx.h"
#include "BehaviorBullet.h"
#include "GameObject.h"
#include "Collider.h"

enum BulletState
{
	cBulletIdle,
};

BehaviorBullet::BehaviorBullet()
{
	SetTimer(bulletLifeTimeMax);
	SetNextState(cBulletIdle);
}

Component * BehaviorBullet::Clone() const
{
	return new BehaviorBullet(*this);
}

void BehaviorBullet::OnEnter()
{
	if (GetCurrentState() == cBulletIdle) {
		GameObject *parent_ = GetParent();
		if (!parent_) return;
		Collider *collider = (Collider*)parent_->GetComponent("Collider");
		if (collider)
			collider->SetCollisionHandler(CollisionHandler);
	}
}

void BehaviorBullet::OnUpdate(float dt)
{
	switch (GetCurrentState()) {
	case cBulletIdle:
		UpdateLifeTimer(dt);
		break;
	}
}

void BehaviorBullet::UpdateLifeTimer(float dt)
{
	if (GetTimer() > 0) {
		SetTimer(GetTimer()-dt);
		if (GetTimer() <= 0) {
			GameObject *parent_ = GetParent();
			if (!parent_) return;
			parent_->Destroy();
		}
	}
}

void BehaviorBullet::CollisionHandler(GameObject & bullet, GameObject & other)
{
	if (other.IsNamed("Asteroid"))
		bullet.Destroy();
}
