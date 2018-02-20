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

typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class GameObjectManager GameObjectManager;
typedef class Component Component;
typedef class SpriteSource SpriteSource;
enum LM_STATE { IDLE, LOADING };

class LevelManager {
public:
	void Init(const char *name);
	void Update(float dt);
	static void UpdateAll(float dt);
	void OnExit();
	void Shutdown();

	void SetNextLevel(const char *name);
	void Restart();
	void Quit();

	static bool LevelExists(const char *name);
	void Load(const char* name);
	static void LoadAbove(const char* name, bool updateLower=false, bool drawLower=true);
	static void UnloadAbove();

	bool IsRunning();

	static LevelManager& GetInstance();
	static LevelManager* GetLowerInstance(unsigned level);
	static void ShutdownInstances();

	void AddComponentType(const char* name, Component* component);

	void AddMesh(const char* name, AEGfxVertexList* mesh);
	void AddTexture(const char* name,  AEGfxTexture* texture);
	void AddSpriteSource(const char* name,  SpriteSource* spriteSource);

	Component *GetComponentType(const char* name);

	AEGfxVertexList* GetMesh(const char* name);
	AEGfxTexture* GetTexture(const char* name);
	SpriteSource* GetSpriteSource(const char* name);

	GameObjectManager* GetObjectManager();

private:
	enum LevelStatus {
		cLevelUpdate,
		cLevelChange,
		cLevelRestart,
		cLevelQuit,
	};

	LevelManager();

	void loadObject(rapidjson::Document& levelDoc);

	static LM_STATE stateCurr, stateNext;
	unsigned int id = 0;

	LevelStatus levelStatus = cLevelUpdate;
	const char *currLevel = "";
	const char *nextLevel = "";

	map<string, Component*> components;

	map<string, AEGfxTexture*> textures;
	map<string, AEGfxVertexList*> meshes;
	map<string, SpriteSource*> spriteSources;

	GameObjectManager *objectManager;

	static LevelManager *instance;
	static vector<LevelManager*> instances;
};
