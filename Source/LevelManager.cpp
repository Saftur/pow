#include "stdafx.h"

#include <string>
#include <fstream>
#include <Document.h>

#include "LevelManager.h"
#include "GameObject.h"
#include "Trace.h"
#include "Transform.h"
#include "Sprite.h"
#include "Behavior.h"
#include "ColliderCircle.h"
#include "Animation.h"
#include "Physics.h"
#include "GameObjectManager.h"

using namespace std;
using namespace rapidjson;

LM_STATE LevelManager::stateCurr = IDLE;
LM_STATE LevelManager::stateNext = IDLE;

void LevelManager::Update(float dt)
{
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
	
	assert(levelDoc.IsObject());

	//int id = 0;

	stateNext = LOADING;

	while (stateNext != IDLE)
		loadObject(levelDoc);
}

void LevelManager::loadObject(Document& levelDoc)
{
	// Check if we're done loading.
	if (!levelDoc.HasMember(to_string(id).c_str()))
	{
		stateNext = IDLE;
		return;
	}

	// Load the game object.
	Value& v = levelDoc[to_string(id).c_str()];

	// If this fails, we weren't loading an object.
	assert(levelDoc[to_string(id).c_str()].IsObject());

	// Create the game object.
	GameObject* go = new GameObject(v["Name"].GetString());

	id++;

	// Load the transform component.
	if (v.HasMember("Transform"))
	{
		Transform* t = new Transform(0, 0);
		t->Load(v["Transform"]);

		go->AddComponent(t);
	}

	// Load the sprite component.
	if (v.HasMember("Sprite"))
	{
		Sprite* s = new Sprite();
		s->Load(v["Sprite"]);

		go->AddComponent(s);
	}

	// Load the animation component.
	if (v.HasMember("Animation"))
	{
		Animation* s = new Animation();
		s->Load(v["Animation"]);

		go->AddComponent(s);
	}

	// Load the physics component.
	if (v.HasMember("Physics"))
	{
		Physics* s = new Physics();
		s->Load(v["Physics"]);

		go->AddComponent(s);
	}

	// Load the collider component.
	if (v.HasMember("Collider"))
	{
		Collider* s = nullptr;

		if (!strcmp(v["Collider"]["Type"].GetString(), "Circle"))
		{
			s = new ColliderCircle(v["Collider"]["Radius"].GetFloat());
		}

		s->Load(v["Collider"]);

		go->AddComponent(s);
	}

	// Load the behavior component.
	/*if (v.HasMember("Behavior"))
	{
		Behavior* s = new Behavior();
		s->Load(v["Behavior"]);

		go->AddComponent(s);
	}*/

	/// TODO: Behaviors.

	GameObjectManager::GetInstance().Add(*go);
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
	return meshes[name];
}

AEGfxTexture* LevelManager::GetTexture(const char* name)
{
	return textures[name];
}

SpriteSource* LevelManager::GetSpriteSource(const char* name)
{
	return spriteSources[name];
}

LevelManager& LevelManager::GetInstance()
{
	static LevelManager m;
	return m;
}