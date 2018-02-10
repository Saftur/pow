#pragma once

#include <map>
using std::map;
using std::pair;
#include <string>
using std::string;

typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class SpriteSource SpriteSource;

class LevelManager {
public:
	void Init();
	void Load(const char *name);
	void Update(float dt);
	void Shutdown();

	void SetNextLevel(const char *name);
	void Restart();
	void Quit();

	bool IsRunning();

	static LevelManager& GetInstance();

private:
	enum LevelStatus {
		cLevelUpdate,
		cLevelChange,
		cLevelRestart,
		cLevelQuit,
	};

	LevelManager();

	LevelStatus levelStatus = cLevelChange;
	const char *currLevel = "";
	const char *nextLevel = "";

	map<const char*, AEGfxTexture*> textures;
	map<const char*, AEGfxVertexList*> meshes;
	map<const char*, SpriteSource*> spriteSources;
};