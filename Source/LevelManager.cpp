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
#include "Collider.h"
#include "Animation.h"
#include "Physics.h"
#include "GameObjectManager.h"

using namespace std;
using namespace rapidjson;

LM_STATE LevelManager::stateCurr = IDLE;
LM_STATE LevelManager::stateNext = IDLE;

Document levelDoc;

void LevelManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	switch (stateCurr)
	{
	case LOADING:
		loadObject();
		break;
	} 

	if (stateCurr != stateNext)
		stateCurr = stateNext;
}

void LevelManager::Load(const char* fileName)
{
	// Create the path string.
	string path = "Levels\\";
	path.append(fileName);
	path.append(".json");

	ifstream file(path);
	string line, contents;

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
}

void LevelManager::loadObject()
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

	UNREFERENCED_PARAMETER(go);

	// Load the transform component.
	if (v.HasMember("Transform"))
	{
		Value& t = v["Transform"];
		Value& tmp = t["Translation"];

		Transform* transform = new Transform(tmp[0].GetFloat(), tmp[1].GetFloat());

		tmp = t["Scale"];
		transform->SetScale(Vector2D(tmp[0].GetFloat(), tmp[1].GetFloat()));

		tmp = t["Rotation"];
		transform->SetRotation(tmp.GetFloat());

		go->AddComponent(transform);
	}

	// Load the sprite component.
	// Load the animation component.
	// Load the physics component.
	// Load the collider component.
	// Load the behavior component.
}

LevelManager& LevelManager::GetInstance()
{
	static LevelManager m;
	return m;
}