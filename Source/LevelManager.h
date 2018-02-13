#pragma once

#include <map>
using std::map;
using std::pair;
#include <string>
using std::string;
#include "rapidjson.h"
#include "document.h"

typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class SpriteSource SpriteSource;
enum LM_STATE { IDLE, LOADING };

class LevelManager {
public:
	void Init(const char *name);
	void Update(float dt);
	void Shutdown();

	void SetNextLevel(const char *name);
	void Restart();
	void Quit();

	void Load(const char* fileName);

	bool IsRunning();

	static LevelManager& GetInstance();

	void AddMesh(const char* name, AEGfxVertexList* mesh);
	void AddTexture(const char* name,  AEGfxTexture* texture);
	void AddSpriteSource(const char* name,  SpriteSource* spriteSource);

	AEGfxVertexList* GetMesh(const char* name);
	AEGfxTexture* GetTexture(const char* name);
	SpriteSource* GetSpriteSource(const char* name);

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

	map<const char*, AEGfxTexture*> textures;
	map<const char*, AEGfxVertexList*> meshes;
	map<const char*, SpriteSource*> spriteSources;
};