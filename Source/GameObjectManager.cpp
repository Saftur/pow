//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.cpp
// Author(s):	Mark Culp
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectManager.h"
#include "AEEngine.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

GameObjectManager::GameObjectManager()
{
	
}

// Initialize the game object manager.
// (NOTE: This function should initialize the lists for both active game objects
//    and the archetype objects.)
void GameObjectManager::Init(void)
{
	gameObjectActiveList = GameObjectList();
	gameObjectArchetypes = GameObjectList();

	gameObjectActiveList.objectMax = objectListSize;
	gameObjectArchetypes.objectMax = objectListSize;
}

// Update all objects in the active game objects list.
// (NOTE: After a game object has been updated, if it has been flagged as
//    destroyed, then it must be removed from the list and freed properly.
//    Additionally, the count of active objects must be reduced by 1.)
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectManager::Update(float dt)
{
	for (int i = 0; i < objectListSize; i++)
	{
		if (gameObjectActiveList.objectList[i])
		{
			if (!gameObjectActiveList.objectList[i]->IsDestroyed())
				gameObjectActiveList.objectList[i]->Update(dt);
			else
			{
				delete(gameObjectActiveList.objectList[i]);

				for (int j = i; j < objectListSize - 1; j++)
				{
					gameObjectActiveList.objectList[j] = gameObjectActiveList.objectList[j + 1];
				}
				
				gameObjectActiveList.objectList[objectListSize - 1] = NULL;

				gameObjectActiveList.objectCount--;
			}
		}
	}
}

// Check collisions between pairs of objects.
void GameObjectManager::CheckCollisions()
{
	for (unsigned i = 0; i < gameObjectActiveList.objectCount; i++)
	{
		if (!gameObjectActiveList.objectList[i]->IsDestroyed() && gameObjectActiveList.objectList[i]->GetCollider())
		{
			for (unsigned j = i + 1; j < gameObjectActiveList.objectCount; j++)
			{
				if (!gameObjectActiveList.objectList[j]->IsDestroyed() && gameObjectActiveList.objectList[j]->GetCollider())
					gameObjectActiveList.objectList[i]->GetCollider()->CheckCollision(*gameObjectActiveList.objectList[j]->GetCollider());
			}
		}
	}
}

// Draw all game objects in the active game object list.
void GameObjectManager::Draw(void)
{
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	for (int i = 0; i < objectListSize; i++)
	{
		if (gameObjectActiveList.objectList[i])
		{
			if (!gameObjectActiveList.objectList[i]->IsDestroyed())
				gameObjectActiveList.objectList[i]->Draw();
		}
	}
	Transform::SetCamIsDirty(false);
}

// Shutdown the game object manager.
// (NOTE: This means removing all game objects from both the active and
//	  archetype game object lists.  Make sure that the object counts are
//	  properly updated in both situations.)
void GameObjectManager::Shutdown(void)
{
	for (int i = 0; i < objectListSize; i++)
	{
		if (gameObjectActiveList.objectList[i])
		{
			delete(gameObjectActiveList.objectList[i]);

			gameObjectActiveList.objectList[i] = NULL;
			gameObjectActiveList.objectCount--;
		}

		if (gameObjectArchetypes.objectList[i])
		{
			delete(gameObjectArchetypes.objectList[i]);

			gameObjectArchetypes.objectList[i] = NULL;
			gameObjectArchetypes.objectCount--;
		}
	}
}

// Add a game object to the active game object list.
// (Hint: This function and the GameObjectManagerAddArchetype functions require
//    the same code.  Consider creating a single function that adds an object
//	  to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManager::Add(GameObject& gameObject)
{
	if (gameObjectActiveList.objectCount == gameObjectActiveList.objectMax)
		return;

	gameObjectActiveList.objectList[gameObjectActiveList.objectCount++] = &gameObject;
}

// Add a game object to the game object archetype list.
// (Hint: This function and the GameObjectManagerAdd functions require
//    the same code.  Consider creating a single, private function that adds
//	 an object to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManager::AddArchetype(GameObject& gameObject)
{
	if (gameObjectArchetypes.objectCount == gameObjectArchetypes.objectMax)
		return;

	gameObjectArchetypes.objectList[gameObjectArchetypes.objectCount++] = &gameObject;
}

// Returns a pointer to the first active game object matching the specified name.
// (Hint: This function and the GameObjectManagerGetObjectByName functions require
//    the same code.  Consider creating a single, private function that searches
//	  for the named game object in a list that is passed as a parameter.)
// Params:
//	 name = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return NULL.
GameObject* GameObjectManager::GetObjectByName(const char* name)
{
	for (int i = 0; i < objectListSize; i++)
	{
		if (gameObjectActiveList.objectList[i])
		{
			if (gameObjectActiveList.objectList[i]->IsNamed(name))
				return gameObjectActiveList.objectList[i];
		}
	}

	return NULL;
}

vector<GameObject*> GameObjectManager::GetObjectsByName(const char * name)
{
	vector<GameObject*> objects;

	for (int i = 0; i < objectListSize; i++)
	{
		if (gameObjectActiveList.objectList[i])
		{
			if (gameObjectActiveList.objectList[i]->IsNamed(name))
				objects.push_back(gameObjectActiveList.objectList[i]);
		}
	}

	return objects;
}

// Returns a pointer to the first game object archetype matching the specified name.
// (Hint: This function and the GameObjectManagerGetObjectByName functions require
//    the same code.  Consider creating a single, private function that searches
//	  for the named game object in a list that is passed as a parameter.)
// Params:
//	 name = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return NULL.
GameObject* GameObjectManager::GetArchetype(const char* name)
{
	for (int i = 0; i < objectListSize; i++)
	{
		if (gameObjectArchetypes.objectList[i])
		{
			if (gameObjectArchetypes.objectList[i]->IsNamed(name))
				return gameObjectArchetypes.objectList[i];
		}
	}

	return NULL;
}

// Returns a reference to the singleton instance of the GameObjectManager.
GameObjectManager& GameObjectManager::GetInstance()
{
	static GameObjectManager obj;

	return obj;
}