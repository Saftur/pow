#include "stdafx.h"
#include "BehaviorAsteroid.h"
#include <AEEngine.h>
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "Collider.h"
#include "Trace.h"
#include "Random.h"
#include "GameStateAsteroids.h"
#include "Teleporter.h"

enum AsteroidState
{
	cAsteroidIdle,
};

typedef enum
{
	cAsteroidSmall,
	cAsteroidMedium,
	cAsteroidLarge,

} AsteroidSize;

typedef enum
{
	cAsteroidOriginTlc,
	cAsteroidOriginTrc,
	cAsteroidOriginBlc,
	cAsteroidOriginBrc,
	cAsteroidOriginCount,

} AsteroidOrigin;

BehaviorAsteroid::BehaviorAsteroid() :
		size(cAsteroidLarge)
{
	SetNextState(cAsteroidIdle);
}

Component * BehaviorAsteroid::Clone() const
{
	return new BehaviorAsteroid(*this);
}

void BehaviorAsteroid::OnEnter()
{
	if (GetCurrentState() == cAsteroidIdle) {
		origin = RandomRange(0, 3);
		SetPosition();
		SetVelocity();
		GameObject *parent_ = GetParent();
		if (parent_) {
			Collider *collider = (Collider*)parent_->GetComponent("Collider");
			if (collider)
				collider->SetCollisionHandler(CollisionHandler);
		}
	}
}

void BehaviorAsteroid::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	TeleporterUpdateObject(*parent_);
}

void BehaviorAsteroid::CollisionHandler(GameObject & asteroid, GameObject & other)
{
	if (other.IsNamed("Bullet") || other.IsNamed("Spaceship")) {
		BehaviorAsteroid* behavior = (BehaviorAsteroid*)asteroid.GetComponent("Behavior");

		// Calculate score based on size of asteroid
		float scoreValue = 20;
		for (int sizeCategory = behavior->size; sizeCategory > 0; --sizeCategory)
			scoreValue /= 2.0f;

		// Add value to player's score
		GameStateAsteroids::IncreaseScore((int)scoreValue);

		// If asteroid is large, create smaller asteroids
		behavior->SpawnNewAsteroids();

		//Call GameObjectDestroy with gameObject1
		asteroid.Destroy();
	}
}

void BehaviorAsteroid::SetPosition()
{
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	if (size == cAsteroidLarge) {
		Transform *transform = (Transform*)parent_->GetComponent("Transform");
		Trace::GetInstance().GetStream() << origin << std::endl;
		if (transform) {
			Vector2D pos;
			switch (origin) {
			case cAsteroidOriginTlc:
				pos = { AEGfxGetWinMinX(), AEGfxGetWinMaxY() };
				break;
			case cAsteroidOriginTrc:
				pos = { AEGfxGetWinMaxX(), AEGfxGetWinMaxY() };
				break;
			case cAsteroidOriginBlc:
				pos = { AEGfxGetWinMinX(), AEGfxGetWinMinY() };
				break;
			case cAsteroidOriginBrc:
				pos = { AEGfxGetWinMaxX(), AEGfxGetWinMinY() };
			}
			Trace::GetInstance().GetStream() << pos.X() << ", " << pos.Y() << std::endl;
			transform->SetTranslation(pos);
		}
	}
}

void BehaviorAsteroid::SetVelocity()
{
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	Physics *physics = (Physics*)parent_->GetComponent("Physics");
	if (!physics) return;
	int angle = 0;
	switch (size) {
	case cAsteroidLarge:
		switch (origin) {
		case cAsteroidOriginTlc:
			angle = RandomRange(-80, -10);
			break;
		case cAsteroidOriginTrc:
			angle = RandomRange(-170, -100);
			break;
		case cAsteroidOriginBlc:
			angle = RandomRange(10, 80);
			break;
		case cAsteroidOriginBrc:
			angle = RandomRange(100, 170);
			break;
		}
		break;
	case cAsteroidMedium: case cAsteroidSmall:
		angle = RandomRange(0, 359);
		break;
	}
	physics->SetVelocity(Vector2D::FromAngleDegrees((float)angle) * (float)RandomRange((int)asteroidSpeedMin, (int)asteroidSpeedMax));
}

void BehaviorAsteroid::SpawnNewAsteroids()
{
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	int numAsteroids = 0;
	float scaleFactor = 0.8f;

	switch (size)
	{
	case cAsteroidLarge:
		numAsteroids = RandomRange(2, 3);
		//numAsteroids = 3;
		break;
	case cAsteroidMedium:
		numAsteroids = RandomRange(1, 2);
		//numAsteroids = 2;
		break;
	default:
		// Do nothing
		break;
	}

	for (int i = 0; i < numAsteroids; ++i)
	{
		// Clone asteroid
		GameObject* clone = new GameObject(*parent_);

		// Decrease size category by one
		BehaviorAsteroid* behavior = (BehaviorAsteroid*)clone->GetComponent("Behavior");
		--behavior->size;

		// Reset state
		behavior->SetCurrentState(cBehaviorInvalid);
		behavior->SetNextState(cAsteroidIdle);

		// Scale asteroid down
		Transform* transform = (Transform*)clone->GetComponent("Transform");
		transform->SetScale(transform->GetScale() * scaleFactor);

		// Add to object manager
		GameObjectManager::GetInstance().Add(*clone);
	}
}
