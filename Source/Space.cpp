#include "stdafx.h"
#include "Space.h"

unsigned Space::firstUpdated = 0;
unsigned Space::firstDrawn = 0;
unsigned Space::numLayers = 0;
Space *Space::layers[];

Space::Space(unsigned layerNum, bool updateLower, bool drawLower) : layerNum(layerNum), levelManager(this), gameObjectManager(this) {
}

void Space::Update(float dt) {
	levelManager.Update(dt);
	if (layerNum >= firstUpdated) {
		gameObjectManager.Update(dt);
		gameObjectManager.CheckCollisions();
	}
}

void Space::Draw() {
	if (layerNum >= firstDrawn) {
		for (Camera *cam : cameras)
			gameObjectManager.Draw(cam);
	}
}

void Space::UpdateAll(float dt) {
	for (unsigned i = 0; i < MAX_LAYERS; i++) {
		if (layers[i]) {
			if (layers[i]->levelManager.GetLevelStatus() == LevelManager::lsLevelQuit)
				DeleteLayer(i);
			else layers[i]->Update(dt);
		}
	}
}

void Space::DrawAll() {
	for (unsigned i = 0; i < MAX_LAYERS; i++) {
		if (layers[i]) {
			layers[i]->Draw();
		}
	}
}

GameObjectManager * Space::GetGameObjectManager() {
	return &gameObjectManager;
}

LevelManager * Space::GetLevelManager() {
	return &levelManager;
}

void Space::AddCamera(Camera * cam) {
	if (std::find(cameras.begin(), cameras.end(), cam) == cameras.end())
		cameras.push_back(cam);
}

void Space::DelCamera(Camera * cam) {
	vector<Camera*>::iterator found = std::find(cameras.begin(), cameras.end(), cam);
	if (found != cameras.end())
		cameras.erase(found);
}

Space * Space::LoadLayer(unsigned layer, const char* name, bool updateLower, bool drawLower) {
	if (layer >= MAX_LAYERS) return nullptr;
	if (layers[layer]) DeleteLayer(layer);
	layers[layer] = new Space(layer, updateLower, drawLower);// = new LevelManager();
	//layers[layer]->objectManager = GameObjectManager::InitLayer(layer, updateLower, drawLower);
	layers[layer]->levelManager.Init(name);

	if (!updateLower && layer > firstUpdated)
		firstUpdated = layer;
	if (!drawLower && layer > firstDrawn)
		firstDrawn = layer;

	numLayers++;
	return layers[layer];
}

Space * Space::GetLayer(unsigned layer) {
	return layers[layer];
}

void Space::DeleteLayer(unsigned layer) {
	if (layer >= MAX_LAYERS || !layers[layer]) return;

	if (firstUpdated > 0 && firstUpdated == layer && !layers[layer]->updateLower) {
		do {
			firstUpdated--;
		} while (firstUpdated < MAX_LAYERS && (!layers[firstUpdated] || layers[firstUpdated]->updateLower));
	}
	if (firstDrawn > 0 && firstDrawn == layer && !layers[layer]->updateLower) {
		do {
			firstDrawn--;
		} while (firstDrawn < MAX_LAYERS && (!layers[firstDrawn] || layers[firstDrawn]->updateLower));
	}

	layers[layer]->levelManager.Shutdown();
	layers[layer]->gameObjectManager.Shutdown();
	delete layers[layer];
	layers[layer] = nullptr;
	numLayers--;
}

void Space::ShutdownLayers() {
	for (unsigned i = 0; i < MAX_LAYERS; i++)
		DeleteLayer(i);
}

unsigned Space::GetLayerCount() {
	return numLayers;
}
