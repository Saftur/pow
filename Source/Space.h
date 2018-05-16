#pragma once

#include "LevelManager.h"
#include "GameObjectManager.h"

#define MAX_LAYERS 10

typedef class Camera Camera;

class Space {
public:
	Space(unsigned layerNum, bool updateLower, bool drawLower);

	void Update(float dt);
	void Draw();

	static void UpdateAll(float dt);
	static void DrawAll();

	LevelManager *GetLevelManager();
	GameObjectManager *GetGameObjectManager();

	void AddCamera(Camera *cam);
	void DelCamera(Camera *cam);

	static Space *LoadLayer(unsigned layer, const char* name, bool updateLower=false, bool drawLower=true);
	static Space *GetLayer(unsigned layer);
	static void DeleteLayer(unsigned layer);
	static void ShutdownLayers();

	static unsigned GetLayerCount();

private:
	unsigned layerNum;
	LevelManager levelManager;
	GameObjectManager gameObjectManager;
	bool updateLower;
	bool drawLower;

	vector<Camera*> cameras;

	// First layer to be updated
	static unsigned firstUpdated;
	// First layer to be drawn
	static unsigned firstDrawn;
	static unsigned numLayers;
	static Space *layers[MAX_LAYERS];
};