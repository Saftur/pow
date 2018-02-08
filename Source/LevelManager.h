#pragma once

#include <map>
using std::map;
using std::pair;
#include <string>
using std::string;

typedef class AEGfxTexture AEGfxTexture;
typedef class AEGfxVertexList AEGfxVertexList;
typedef class SpriteSource SpriteSource;

class LevelManager {
public:
	void Load(string name);
	void Update(float dt);
	void Shutdown();

	void SetNextLevel(const char *name);

	static LevelManager& GetInstance();

private:
	LevelManager();

	bool changeLevel = true;
	string currLevel = "";
	string nextLevel = "";

	map<const char*, AEGfxTexture*> textures;
	map<const char*, AEGfxVertexList*> meshes;
	map<const char*, SpriteSource*> spriteSources;
};