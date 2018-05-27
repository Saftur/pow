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
typedef class Camera Camera;
typedef class Component Component;
typedef class SpriteSource SpriteSource;
typedef class Space Space;
enum LM_STATE { IDLE, LOADING };

class LevelManager {
public:
	enum LevelStatus {
		lsLevelUpdate,
		lsLevelChange,
		lsLevelRestart,
		lsLevelQuit,
	};

	static void StaticInit();
	void Update(float dt);
	void Shutdown();
	static void StaticShutdown();

	void SetNextLevel(string name, map<string, void*> loadVars = {});
	void Restart();
	void Quit();

	static bool LevelExists(const char *name);
	void Load(string name, map<string, void*> loadVars = {});

	LevelStatus GetLevelStatus() const;
	bool IsRunning();

	void AddMesh(const char* name, AEGfxVertexList* mesh);
	void AddTexture(const char* name,  AEGfxTexture* texture);
	void AddSpriteSource(const char* name,  SpriteSource* spriteSource);

	AEGfxVertexList* GetMesh(const char* name);
	AEGfxTexture* GetTexture(const char* name);
	SpriteSource* GetSpriteSource(const char* name);

	void *GetLoadVar(const char *name);

	static Component *GetComponentType(const char* name);
	static void AddComponentType(const char* name, Component* component);

	Space* GetSpace();
	GameObjectManager* GetGameObjectManager();

private:
	enum ObjType {
		otGameObject,
		otSpriteSource,
		otMesh
	};

	LevelManager(Space *space);

	void loadObject(rapidjson::Document& levelDoc);

	static LM_STATE stateCurr, stateNext;
	unsigned int id = 0;

	LevelStatus levelStatus = lsLevelUpdate;
	string currLevel = "";
	string nextLevel = "";

	map<string, AEGfxTexture*> textures;
	map<string, AEGfxVertexList*> meshes;
	map<string, SpriteSource*> spriteSources;

	map<string, void*> loadVars;

	Space *space;

	static LevelManager *loadingLevelManager;

	static map<string, Component*> components;

	friend class Space;
};
