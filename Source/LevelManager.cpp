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
#include "ColliderBox.h"
#include "ColliderCircle.h"
#include "Animation.h"
#include "Physics.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "Space.h"
#include "Tilemap.h"
#include "Rendertext.h"
#include "Button.h"
#include "CompList.h"
#include "Mesh.h"

using namespace std;
using namespace rapidjson;

LM_STATE LevelManager::stateCurr = IDLE;
LM_STATE LevelManager::stateNext = IDLE;

LevelManager *LevelManager::loadingLevelManager = nullptr;

map<string, Component*> LevelManager::components;

void LevelManager::StaticInit()
{
	AddComponentType("Transform", new Transform(0, 0));
	AddComponentType("Sprite", new Sprite());
	AddComponentType("Animation", new Animation());
	AddComponentType("Physics", new Physics());
	AddComponentType("ColliderBox", new ColliderBox());
	AddComponentType("ColliderCircle", new ColliderCircle(0));
	AddComponentType("Camera", new Camera());
	AddComponentType("Tilemap", new Tilemap());
	AddComponentType("Text", new Text());
	AddComponentType("Button", new Button());
	CompList::List();
}

void LevelManager::Init(const char *name)
{
	SetNextLevel(name);
}

void LevelManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (levelStatus) {
		Shutdown();

		if (levelStatus != lsLevelQuit) {
			Load(nextLevel);

			currLevel = nextLevel;
			levelStatus = lsLevelUpdate;
		}
	}
}

void LevelManager::Shutdown()
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
	GetGameObjectManager()->Shutdown();
}

void LevelManager::StaticShutdown() {
	//Shutdown();
	for (pair<string, Component*> p : components) {
		delete p.second;
	}
	components.clear();
}

void LevelManager::SetNextLevel(const char *name) {
	if (!name) return;
	nextLevel = name;
	levelStatus = lsLevelChange;
}

void LevelManager::Restart()
{
	levelStatus = lsLevelRestart;
}

void LevelManager::Quit()
{
	levelStatus = lsLevelQuit;
}

LevelManager::LevelStatus LevelManager::GetLevelStatus() const {
	return levelStatus;
}

bool LevelManager::IsRunning()
{
	return levelStatus != lsLevelQuit;
}

bool LevelManager::LevelExists(const char * name)
{
	string path = "Data\\Levels\\";
	path.append(name);
	path.append(".json");
	if (FILE *file = fopen(path.c_str(), "r")) {
		fclose(file);
		return true;
	} else return false;
}

void LevelManager::Load(const char* name)
{
	// Create the path string.
	string path = "Data\\Levels\\";
	path.append(name);
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
	
	assert(levelDoc.IsArray());

	stateNext = LOADING;

	loadingLevelManager = this;
	id = 0;
	while (stateNext != IDLE)
		loadObject(levelDoc);

	GetGameObjectManager()->GetObjectsWithFilter([](GameObject *obj) {
		obj->PostLoadInit();
		return false;
	});
}

void LevelManager::loadObject(Document& levelDoc)
{
	// Check if we're done loading.
	if (id >= levelDoc.GetArray().Size()) {
		stateNext = IDLE;
		loadingLevelManager = nullptr;
		return;
	}

	// Load the game object.
	Value& v = levelDoc[id];

	// If this fails, we weren't loading an object.
	assert(v.IsObject());

	// Create the game object.
	ObjType type = otGameObject;
	if (v.HasMember("Type") && v["Type"].IsString()) {
		if (strcmp(v["Type"].GetString(), "SpriteSource") == 0)
			type = otSpriteSource;
		else if (strcmp(v["Type"].GetString(), "Mesh") == 0)
			type = otMesh;
	}
	switch (type) {
	case otGameObject: {
		GameObject* go = new GameObject(v["Name"].GetString(), space);

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
			GetGameObjectManager()->AddArchetype(*go);
		else GetGameObjectManager()->Add(*go);
	}
		break;
	case otSpriteSource: {
		// Create and add a sprite source.
		SpriteSource* ss = new SpriteSource(0, 0, nullptr, this);

		ss->Load(v);

		// Add the sprite source to the map.
		AddSpriteSource(v["Name"].GetString(), ss);
	}
		break;
	case otMesh: {
		// Create and add a mesh.
		rapidjson::Value& tmp1 = v["HalfSize"];
		rapidjson::Value& tmp2 = v["UV"];

		AEGfxVertexList *mesh = MeshCreateQuad(tmp1[0].GetFloat(), tmp1[1].GetFloat(), tmp2[0].GetFloat(), tmp2[1].GetFloat());

		// Add the mesh to the map.
		AddMesh(v["Name"].GetString(), mesh);
	}
		break;
	}

	id++;
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

Component * LevelManager::GetComponentType(const char * name)
{
	return components.count(name) > 0 ? components.at(name) : nullptr;
}

void LevelManager::AddComponentType(const char * name, Component * component)
{
	components[name] = component;
}

Space * LevelManager::GetSpace() {
	return space;
}

GameObjectManager * LevelManager::GetGameObjectManager()
{
	return space->GetGameObjectManager();
}

LevelManager::LevelManager(Space *space) : space(space)
{
}
