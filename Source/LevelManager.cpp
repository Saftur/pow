#include "stdafx.h"
#include "LevelManager.h"
#include <AEEngine.h>
#include "SpriteSource.h"

void LevelManager::Init()
{
}

void LevelManager::Load(const char *name)
{
	UNREFERENCED_PARAMETER(name);
}

void LevelManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (levelStatus) {
		Shutdown();

		if (levelStatus != cLevelQuit) {
			Load(nextLevel);

			currLevel = nextLevel;
			levelStatus = cLevelUpdate;
		}
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
	if (!name) return;
	nextLevel = name;
	levelStatus = cLevelChange;
}

void LevelManager::Restart()
{
	levelStatus = cLevelRestart;
}

void LevelManager::Quit()
{
	levelStatus = cLevelQuit;
}

bool LevelManager::IsRunning()
{
	return levelStatus != cLevelQuit;
}

LevelManager & LevelManager::GetInstance()
{
	static LevelManager instance;
	return instance;
}

LevelManager::LevelManager()
{
}
