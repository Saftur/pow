#include "LevelManager.h"
#include <AEEngine.h>
#include "SpriteSource.h"

void LevelManager::Load(string name)
{
}

void LevelManager::Update(float dt)
{
	if (changeLevel) {
		Shutdown();

		Load(nextLevel);

		currLevel = nextLevel;
		changeLevel = false;
	}
}

void LevelManager::Shutdown()
{
	for (pair<const char*, AEGfxTexture*> p : textures) {
		AEGfxTextureUnload(p.second);
	}
	for (pair<const char*, AEGfxVertexList*> p : meshes) {
		AEGfxMeshFree(p.second);
	}
	for (pair<const char*, SpriteSource*> p : spriteSources) {
		delete p.second;
	}
}

void LevelManager::SetNextLevel(const char *name) {
	nextLevel = name;
	changeLevel = true;
}

LevelManager & LevelManager::GetInstance()
{
	static LevelManager instance;
	return instance;
}
