#include "stdafx.h"
#include "GameObjectManager.h"
#include "AEEngine.h"
#include "Transform.h"
#include "Collider.h"
#include "Button.h"
#include "Trace.h"

//GameObjectManager *GameObjectManager::instance = nullptr;
//vector<GameObjectManager::Layer> GameObjectManager::instances;
GameObjectManager::Layer GameObjectManager::layers[MAX_LAYERS] = {};

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
	// TODO Possibly make var 'first' on class and set it in InitLayer()
	unsigned first = 0;
	for (int i = /*(int)instances.size()*/MAX_LAYERS - 1; i >= 0; i--) {
		/*if (!instances[i].update) {
			first = i + 1;
		}*/
		if (layers[i].instance && !layers[i].updateLower)
			first = i;
	}
	for (unsigned i = first; i < /*instances.size()*/MAX_LAYERS; i++) {
		//instances[i].instance->Update(dt);
		//instances[i].instance->CheckCollisions();
		if (!layers[i].instance) continue;
		layers[i].instance->Update(dt);
		layers[i].instance->CheckCollisions();
	}
	//instance->Update(dt);
	//instance->CheckCollisions();
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
	for (int i = /*(int)instances.size()*/MAX_LAYERS - 1; i >= 0; i--) {
		/*if (!instances[i].draw) {
			first = i + 1;
		}*/
		if (layers[i].instance && !layers[i].drawLower) {
			first = i;
		}
	}
	for (unsigned i = first; i < /*instances.size()*/MAX_LAYERS; i++) {
		//instances[i].instance->Draw();
		if (!layers[i].instance) continue;
		layers[i].instance->Draw();
	}
	//instance->Draw();
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
	Button::Shutdown();
}

GameObject* GameObjectManager::Add(GameObject & gameObject)
{
	GameObject *newObject = new GameObject(gameObject);
	activeList.push_back(newObject);
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

/*GameObjectManager & GameObjectManager::GetInstance()
{
	//static GameObjectManager instance;
	if (!instance)
		instance = new GameObjectManager();
	return *instance;
}*/

GameObjectManager *GameObjectManager::InitLayer(unsigned layer, bool updateLower, bool drawLower)
{
	//instances.push_back({updateLower, drawLower, instance});
	//instance = new GameObjectManager();
	//instance->Init();
	if (layer >= MAX_LAYERS) return nullptr;
	layers[layer] = { updateLower, drawLower, new GameObjectManager() };
	layers[layer].instance->Init();
	return layers[layer].instance;
}

GameObjectManager * GameObjectManager::GetLayer(unsigned layer)
{
	return layers[layer].instance;
}

void GameObjectManager::DeleteLayer(unsigned layer)
{
	//if (instances.size() == 0) return;
	if (layer >= MAX_LAYERS || !layers[layer].instance) return;
	//instance->Shutdown();
	layers[layer].instance->Shutdown();
	//delete instance;
	delete layers[layer].instance;
	//instance = instances[instances.size()-1].instance;
	//instances.pop_back();
	layers[layer].instance = nullptr;
}

void GameObjectManager::ShutdownLayers()
{
	//instance->Shutdown();
	//delete instance;
	for (unsigned i = 0; i < MAX_LAYERS; i++) {
		if (layers[i].instance) {
			layers[i].instance->Shutdown();
			delete layers[i].instance;
			layers[i].instance = nullptr;
		}
	}
	//instances.clear();
}

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}
