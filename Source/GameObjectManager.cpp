#include "stdafx.h"
#include "GameObjectManager.h"
#include "AEEngine.h"
#include "Transform.h"
#include "Collider.h"
#include "Button.h"

void GameObjectManager::Init(void)
{
	gameObjectActiveList = { 0, objectListSize };
	gameObjectArchetypes = { 0, objectListSize };
}

void GameObjectManager::Update(float dt)
{
	GameObject *gameObject;
	for (unsigned int i = 0; i < gameObjectActiveList.objectCount; i++) {
		gameObject = gameObjectActiveList.objectList[i];
		if (!gameObject->IsDestroyed())
			gameObject->Update(dt);
	}
	for (unsigned int i = 0; i < gameObjectActiveList.objectCount; i++) {
		gameObject = gameObjectActiveList.objectList[i];
		if (gameObject->IsDestroyed()) {
			delete gameObject;
			for (unsigned int a = i + 1; a < gameObjectActiveList.objectCount; a++)
				gameObjectActiveList.objectList[a - 1] = gameObjectActiveList.objectList[a];
			i--;
			gameObjectActiveList.objectCount--;
		}
	}
}

void GameObjectManager::CheckCollisions()
{
	Collider *collider1, *collider2;
	for (unsigned int i = 0; i < gameObjectActiveList.objectCount; i++) {
		if (gameObjectActiveList.objectList[i]->IsDestroyed())
			continue;
		collider1 = (Collider*)gameObjectActiveList.objectList[i]->GetComponent("Collider");
		if (!collider1) continue;
		for (unsigned int j = i + 1; j < gameObjectActiveList.objectCount; j++) {
			if (gameObjectActiveList.objectList[j]->IsDestroyed())
				continue;
			collider2 = (Collider*)gameObjectActiveList.objectList[j]->GetComponent("Collider");
			if (!collider2) continue;
			collider1->CheckCollision(*collider2);
		}
	}
}

void GameObjectManager::Draw(void)
{
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	
	for (unsigned int i = 0; i < gameObjectActiveList.objectCount; i++)
		gameObjectActiveList.objectList[i]->Draw();
	
	Transform::SetCamIsDirty(false);
}

void GameObjectManager::Shutdown(void)
{
	for (unsigned int i = 0; i < gameObjectActiveList.objectCount; i++)
		delete gameObjectActiveList.objectList[i];
	for (unsigned int i = 0; i < gameObjectArchetypes.objectCount; i++)
		delete gameObjectArchetypes.objectList[i];

	gameObjectActiveList.objectCount = 0;
	gameObjectArchetypes.objectCount = 0;
	Button::Shutdown();
}

void GameObjectManager::Add(GameObject & gameObject)
{
	unsigned int &objectCount = gameObjectActiveList.objectCount;
	if (objectCount < gameObjectActiveList.objectMax)
		gameObjectActiveList.objectList[objectCount++] = &gameObject;
}

void GameObjectManager::AddArchetype(GameObject & gameObject)
{
	unsigned int &objectCount = gameObjectArchetypes.objectCount;
	if (objectCount < gameObjectArchetypes.objectMax)
		gameObjectArchetypes.objectList[objectCount++] = &gameObject;
}

GameObject * GameObjectManager::GetObjectByName(const char * name) const
{
	for (unsigned int i = 0; i < gameObjectActiveList.objectCount; i++)
		if (!gameObjectActiveList.objectList[i]->IsDestroyed() && gameObjectActiveList.objectList[i]->IsNamed(name))
			return gameObjectActiveList.objectList[i];
	return nullptr;
}

vector<GameObject*> GameObjectManager::GetObjectsByName(const char * name)
{
	vector<GameObject*> objects;

	for (int i = 0; i < objectListSize; i++)
	{
		if (!gameObjectActiveList.objectList[i]->IsDestroyed() && gameObjectActiveList.objectList[i]->IsNamed(name))
			objects.push_back(gameObjectActiveList.objectList[i]);
	}

	return objects;
}

GameObject * GameObjectManager::GetArchetype(const char * name) const
{
	for (unsigned int i = 0; i < gameObjectArchetypes.objectCount; i++)
		if (gameObjectArchetypes.objectList[i]->IsNamed(name))
			return gameObjectArchetypes.objectList[i];
	return nullptr;
}

GameObjectManager & GameObjectManager::GetInstance()
{
	static GameObjectManager instance;
	return instance;
}

GameObjectManager::GameObjectManager()
{
}
