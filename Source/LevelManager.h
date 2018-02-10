#pragma once
#include <map>
#include "AEEngine.h"
#include "SpriteSource.h"
#include "rapidjson.h"

enum LM_STATE { IDLE, LOADING };

class LevelManager
{
public:
	void Init();
	void Update(float dt);
	void Shutdown();

	void Load(const char* fileName);
	static LevelManager& GetInstance();

	void AddMesh(const char* name, AEGfxVertexList* mesh);
	void AddTexture(const char* name,  AEGfxTexture* texture);
	void AddSpriteSource(const char* name,  SpriteSource* spriteSource);

	AEGfxVertexList* GetMesh(const char* name);
	AEGfxTexture* GetTexture(const char* name);
	SpriteSource* GetSpriteSource(const char* name);
private:
	void loadObject(rapidjson::Document& levelDoc);

	static LM_STATE stateCurr, stateNext;
	unsigned int id = 0;

	std::map<const char*, AEGfxVertexList*> meshes;
	std::map<const char*, AEGfxTexture*> textures;
	std::map<const char*, SpriteSource*> spriteSources;
};
