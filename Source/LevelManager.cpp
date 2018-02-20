#include "stdafx.h"

#include <string>
#include <fstream>
#include <Document.h>

#include "LevelManager.h"
#include <AEEngine.h>
#include "GameObject.h"
#include "Trace.h"
#include "Transform.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Behavior.h"
#include "ColliderBox.h"
#include "ColliderCircle.h"
#include "Animation.h"
#include "Physics.h"
#include "GameObjectManager.h"
#include "Tilemap.h"
#include "Rendertext.h"
#include "Button.h"
#include "CompList.h"

#include "PauseMenu.h"

using namespace std;
using namespace rapidjson;

LM_STATE LevelManager::stateCurr = IDLE;
LM_STATE LevelManager::stateNext = IDLE;

LevelManager *LevelManager::instance = nullptr;
vector<LevelManager*> LevelManager::instances;

void LevelManager::Init(const char *name)
{
	SetNextLevel(name);
	AddComponentType("Transform", new Transform(0, 0));
	AddComponentType("Sprite", new Sprite());
	AddComponentType("Animation", new Animation());
	AddComponentType("Physics", new Physics());
	AddComponentType("ColliderBox", new ColliderBox());
	AddComponentType("ColliderCircle", new ColliderCircle(0));
	AddComponentType("Tilemap", new Tilemap());
	AddComponentType("Text", new Text());
	AddComponentType("Button", new Button());
	CompList::List();
}

void LevelManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (levelStatus) {
		OnExit();

		if (levelStatus != cLevelQuit) {
			Load(nextLevel);

			currLevel = nextLevel;
			levelStatus = cLevelUpdate;
		}
	}
}

void LevelManager::UpdateAll(float dt)
{
	for (LevelManager *i : instances)
		i->Update(dt);
	instance->Update(dt);
}

void LevelManager::OnExit()
{
	for (pair<string, AEGfxTexture*> p : textures) {
		AEGfxTextureUnload(p.second);
	}
	textures.clear();
	for (pair<string, AEGfxVertexList*> p : meshes) {
		AEGfxMeshFree(p.second);
	}
	meshes.clear();
	for (pair<string, SpriteSource*> p : spriteSources) {
		delete p.second;
	}
	spriteSources.clear();
	GameObjectManager::GetInstance().Shutdown();
}

void LevelManager::Shutdown() {
	OnExit();
	for (pair<string, Component*> p : components) {
		delete p.second;
	}
	components.clear();
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
	//static LevelManager instance;
	if (!instance)
		instance = new LevelManager();
	return *instance;
}

LevelManager * LevelManager::GetLowerInstance(unsigned level)
{
	if (level > instances.size()) return nullptr;
	return instances[instances.size() - level];
}

void LevelManager::ShutdownInstances()
{
	delete instance;
	for (LevelManager *i : instances)
		delete i;
}

void LevelManager::Load(const char* fileName)
{
	// Create the path string.
	string path = "Levels\\";
	path.append(fileName);
	path.append(".json");

	ifstream file(path);
	string line, contents;

	Document levelDoc;

	// Load the file into contents.
	while (getline(file, line))
	{
		contents.append(line);
	}

	// Parse it into a levelDoc.
	levelDoc.Parse(contents.c_str());
	
	//assert(levelDoc.IsObject());
	assert(levelDoc.IsArray());

	//int id = 0;

	stateNext = LOADING;

	id = 0;
	while (stateNext != IDLE)
		loadObject(levelDoc);
}

void LevelManager::LoadAbove(const char * fileName, bool updateLower, bool drawLower)
{
	instances.push_back(instance);
	instance = new LevelManager();
	GameObjectManager::NewLayer(updateLower, drawLower);
	instance->Init(fileName);
	//instance->Load(fileName);
}

void LevelManager::UnloadAbove()
{
	if (instances.size() == 0) return;
	instance->Shutdown();
	delete instance;
	instance = instances[instances.size()-1];
	instances.pop_back();
	GameObjectManager::DeleteLayer();
}

void LevelManager::loadObject(Document& levelDoc)
{
	// Check if we're done loading.
	if (id >= levelDoc.GetArray().Size()) {
		stateNext = IDLE;
		return;
	}

	// Load the game object.
	Value& v = levelDoc[id];

	// If this fails, we weren't loading an object.
	assert(v.IsObject());

	// Create the game object.
	GameObject* go = new GameObject(v["Name"].GetString());

	bool archetype = v.HasMember("Archetype") && v["Archetype"].GetType() == rapidjson::Type::kTrueType;

	for (Value::MemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); itr++) {
		Component *c = GetComponentType(itr->name.GetString());
		if (c) {
			c = c->Clone();
			go->AddComponent(c);
			c->Load(itr->value);
		}
	}

	if (v.HasMember("Extras") && v["Extras"].IsArray()) {
		for (Value::ValueIterator itr = v["Extras"].Begin(); itr != v["Extras"].End(); itr++) {
			if (itr->IsObject() && itr->HasMember("CompType") && (*itr)["CompType"].IsString()) {
				Component *c = GetComponentType((*itr)["CompType"].GetString());
				if (c) {
					c = c->Clone();
					go->AddComponent(c);
					c->Load(*itr);
				}
			}
		}
	}

	if (archetype)
		GameObjectManager::GetInstance().AddArchetype(*go);
	else GameObjectManager::GetInstance().Add(*go);

	id++;
}

void LevelManager::AddComponentType(const char * name, Component * component)
{
	components[name] = component;
}

void LevelManager::AddMesh(const char* name, AEGfxVertexList* mesh)
{
	meshes[name] = mesh;
}

void LevelManager::AddTexture(const char* name, AEGfxTexture* texture)
{
	textures[name] = texture;
}

void LevelManager::AddSpriteSource(const char* name, SpriteSource* spriteSource)
{
	spriteSources[name] = spriteSource;
}

Component * LevelManager::GetComponentType(const char * name)
{
	return components.count(name) > 0 ? components.at(name) : nullptr;
}

AEGfxVertexList* LevelManager::GetMesh(const char* name)
{
	return meshes.count(name) > 0 ? meshes.at(name) : nullptr;
}

AEGfxTexture* LevelManager::GetTexture(const char* name)
{
	return textures.count(name) > 0 ? textures.at(name) : nullptr;
}

SpriteSource* LevelManager::GetSpriteSource(const char* name)
{
	return spriteSources.count(name) > 0 ? spriteSources.at(name) : nullptr;
}

LevelManager::LevelManager()
{
}
