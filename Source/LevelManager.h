#pragma once

#include <map>
using std::map;
using std::pair;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include "rapidjson.h"
#include "document.h"

#ifndef MAX_LAYERS
#define MAX_LAYERS 10
#endif

typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class GameObjectManager GameObjectManager;
typedef class Component Component;
typedef class SpriteSource SpriteSource;
enum LM_STATE { IDLE, LOADING };

class LevelManager {
public:
	static void StaticInit();
	void Init(const char *name);
	void Update(float dt);
	static void UpdateAll(float dt);
	void Shutdown();
	static void StaticShutdown();

	void SetNextLevel(const char *name);
	void Restart();
	void Quit();

	static bool LevelExists(const char *name);
	void Load(const char* name);

	bool IsRunning();

	//static LevelManager& GetInstance();
	//static void LoadAbove(const char* name, bool updateLower=false, bool drawLower=true);
	//static void UnloadAbove();
	static void LoadLayer(unsigned layer, const char* name, bool updateLower=false, bool drawLower=true);
	static void UnloadLayer(unsigned layer);
	static LevelManager* GetLoadingLevel();
	static LevelManager* GetLayer(unsigned num);
	static void ShutdownLayers();

	void AddMesh(const char* name, AEGfxVertexList* mesh);
	void AddTexture(const char* name,  AEGfxTexture* texture);
	void AddSpriteSource(const char* name,  SpriteSource* spriteSource);

	AEGfxVertexList* GetMesh(const char* name);
	AEGfxTexture* GetTexture(const char* name);
	SpriteSource* GetSpriteSource(const char* name);

	static Component *GetComponentType(const char* name);
	static void AddComponentType(const char* name, Component* component);

	GameObjectManager* GetObjectManager();

private:
	enum LevelStatus {
		lsLevelUpdate,
		lsLevelChange,
		lsLevelRestart,
		lsLevelQuit,
	};
	enum ObjType {
		otGameObject,
		otSpriteSource,
		otMesh
	};

	LevelManager();

	void loadObject(rapidjson::Document& levelDoc);

	static LM_STATE stateCurr, stateNext;
	unsigned int id = 0;

	LevelStatus levelStatus = lsLevelUpdate;
	const char *currLevel = "";
	const char *nextLevel = "";

	map<string, AEGfxTexture*> textures;
	map<string, AEGfxVertexList*> meshes;
	map<string, SpriteSource*> spriteSources;

	GameObjectManager *objectManager;

	static LevelManager *loadingLevelManager;

	static map<string, Component*> components;

	//static LevelManager *instance;
	//static vector<LevelManager*> instances;
	static LevelManager *layers[MAX_LAYERS];
};
