#include "stdafx.h"
#include "GameObjectManager.h"
#include "AEEngine.h"
#include "Transform.h"
#include "Collider.h"
#include "Button.h"
#include "Trace.h"

GameObjectManager *GameObjectManager::instance = nullptr;
vector<GameObjectManager::GOMLayeredInstance> GameObjectManager::instances;

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

void GameObjectManager::UpdateAll(float dt)
{
	unsigned first = 0;
	for (int i = (int)instances.size() - 1; i >= 0; i--) {
		if (!instances[i].update) {
			first = i + 1;
		}
	}
	for (unsigned i = first; i < instances.size(); i++) {
		instances[i].instance->Update(dt);
		instances[i].instance->CheckCollisions();
	}
	instance->Update(dt);
	instance->CheckCollisions();
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

void GameObjectManager::Draw(void)
{
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	
	for (GameObject *gameObject : activeList)
		gameObject->Draw();
	
	Transform::SetCamIsDirty(false);
}

void GameObjectManager::DrawAll()
{
	unsigned first = 0;
	for (int i = (int)instances.size() - 1; i >= 0; i--) {
		if (!instances[i].draw) {
			first = i + 1;
		}
	}
	for (unsigned i = first; i < instances.size(); i++) {
		instances[i].instance->Draw();
	}
	instance->Draw();
}

void GameObjectManager::Shutdown(void)
{
	for (GameObject *gameObject : activeList)
		delete gameObject;
	for (GameObject *gameObject : archetypes)
		delete gameObject;

	if (!activeList.empty())
		activeList.clear();
	if (!archetypes.empty())
		archetypes.clear();
	Button::Shutdown();
}

void GameObjectManager::Add(GameObject & gameObject)
{
	activeList.push_back(&gameObject);
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

GameObjectManager & GameObjectManager::GetInstance()
{
	//static GameObjectManager instance;
	if (!instance)
		instance = new GameObjectManager();
	return *instance;
}

void GameObjectManager::NewLayer(bool updateLower, bool drawLower)
{
	instances.push_back({updateLower, drawLower, instance});
	instance = new GameObjectManager();
	instance->Init();
}

void GameObjectManager::DeleteLayer()
{
	if (instances.size() == 0) return;
	instance->Shutdown();
	delete instance;
	instance = instances[instances.size()-1].instance;
	instances.pop_back();
}

void GameObjectManager::ShutdownInstances()
{
	delete instance;
	for (GOMLayeredInstance i : instances) {
		i.instance->Shutdown();
		delete i.instance;
	}
	instances.clear();
}

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}
