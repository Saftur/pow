#include "stdafx.h"
#include "GameObjectManager.h"
#include "Space.h"
#include "AEEngine.h"
#include "Collider.h"
#include "Button.h"
#include "Trace.h"

void GameObjectManager::Init(void)
{
}

void GameObjectManager::Update(float dt)
{
	GameObject *gameObject;
	unsigned int numObj = activeList.size();
	for (unsigned int i = 0; i < numObj; i++) {
		gameObject = activeList[i];
		if (!gameObject->IsDestroyed())
			gameObject->Update(dt);
	}
	for (unsigned i = 0; i < activeList.size(); i++) {
		gameObject = activeList[i];
		if (gameObject->IsDestroyed()) {
			if (gameObject->CheckDestroyNow()) {
				delete gameObject;
				activeList.erase(activeList.begin() + i);
				i--;
			} else gameObject->SetDestroyNext();
		}
	}
}

void GameObjectManager::CheckCollisions()
{
	Collider *collider1, *collider2;
	for (unsigned i = 0; i < activeList.size(); i++) {
		if (activeList[i]->IsDestroyed())
			continue;
		collider1 = (Collider*)activeList[i]->GetComponent("Collider");
		if (!collider1) continue;
		for (unsigned j = i + 1; j < activeList.size(); j++) {
			if (activeList[j]->IsDestroyed())
				continue;
			collider2 = (Collider*)activeList[j]->GetComponent("Collider");
			if (!collider2) continue;
			collider1->CheckCollision(*collider2);
		}
	}
}

void GameObjectManager::Draw(Camera * cam)
{
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	
	for (GameObject *gameObject : activeList)
		gameObject->Draw(cam);
}

void GameObjectManager::Shutdown(void)
{
	for (GameObject *gameObject : activeList) {
		gameObject->Destroy();
		delete gameObject;
	}
	for (GameObject *gameObject : archetypes)
		delete gameObject;

	if (!activeList.empty())
		activeList.clear();
	if (!archetypes.empty())
		archetypes.clear();
}

GameObject * GameObjectManager::Add(GameObject & gameObject) {
	activeList.push_back(&gameObject);
	gameObject.InstanceInit();
	return &gameObject;
}

GameObject* GameObjectManager::AddNew(GameObject & gameObject)
{
	GameObject *newObject = new GameObject(gameObject);
	activeList.push_back(newObject);
	newObject->InstanceInit();
	return newObject;
}

void GameObjectManager::AddArchetype(GameObject & gameObject)
{
	archetypes.push_back(&gameObject);
}

GameObject * GameObjectManager::GetObjectByName(const char * name) const
{
	for (GameObject *gameObject : activeList)
		if (!gameObject->IsDestroyed() && gameObject->IsNamed(name))
			return gameObject;
	return nullptr;
}

vector<GameObject*> GameObjectManager::GetObjectsByName(const char * name)
{
	vector<GameObject*> objects;

	for (GameObject *gameObject : activeList)
		if (!gameObject->IsDestroyed() && gameObject->IsNamed(name))
			objects.push_back(gameObject);

	return objects;
}

GameObject * GameObjectManager::GetArchetype(const char * name) const
{
	for (GameObject *gameObject : archetypes)
		if (gameObject->IsNamed(name))
			return gameObject;
	return nullptr;
}

Space * GameObjectManager::GetSpace() {
	return space;
}

LevelManager * GameObjectManager::GetLevelManager() {
	return space->GetLevelManager();
}

GameObjectManager::GameObjectManager(Space *space) : space(space)
{
}

GameObjectManager::~GameObjectManager()
{
}
