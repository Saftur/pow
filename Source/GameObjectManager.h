//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.h
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright � 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "GameObject.h"
#include <vector>
using std::vector;
#include "Camera.h"

/*#ifndef MAX_LAYERS
#define MAX_LAYERS 10
#endif*/

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Space Space;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public functions declared in the header.
class GameObjectManager
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Initialize the game object manager.
	// (NOTE: This function should initialize the lists for both active game objects
	//    and the archetype objects.)
	void Init(void);

	// Update all objects in the active game objects list.
	// (NOTE: After a game object has been updated, if it has been flagged as
	//    destroyed, then it must be removed from the list and freed properly.
	//    Additionally, the count of active objects must be reduced by 1.)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);
	// Update all current instances, if to be updated
	// Also calls CheckCollisions for all of them
	//static void UpdateAll(float dt);

	// Check collisions between pairs of objects.
	void CheckCollisions();

	// Draw all game objects in the active game object list.
	void Draw(Camera * cam);
	// Draw all current instances, if to be drawn
	//static void DrawAll(Camera * cam);

	// Shutdown the game object manager.
	// (NOTE: This means removing all game objects from both the active and
	//	  archetype game object lists.  Make sure that the object counts are
	//	  properly updated in both situations.)
	void Shutdown(void);

	// Add a game object to the active game object list.
	// (Hint: This function and the GameObjectManagerAddArchetype functions require
	//    the same code.  Consider creating a single function that adds an object
	//	  to a list that is passed as a parameter.)
	// Params:
	//	 gameObject = Reference to the game object to be added to the list.
	GameObject* Add(GameObject& gameObject);

	// Add a game object to the game object archetype list.
	// (Hint: This function and the GameObjectManagerAdd functions require
	//    the same code.  Consider creating a single, private function that adds
	//	 an object to a list that is passed as a parameter.)
	// Params:
	//	 gameObject = Reference to the game object to be added to the list.
	void AddArchetype(GameObject& gameObject);

	// Returns a pointer to the first active game object matching the specified name.
	// (Hint: This function and the GameObjectManagerGetObjectByName functions require
	//    the same code.  Consider creating a single, private function that searches
	//	  for the named game object in a list that is passed as a parameter.)
	// Params:
	//	 name = The name of the object to be returned, if found.
	// Returns:
	//   If the named object is found,
	//	   then return the pointer to the named game object,
	//	   else return nullptr.
	GameObject* GetObjectByName(const char* name) const;
	
	vector<GameObject*> GetObjectsByName(const char* name);

	template<typename Func>
	vector<GameObject*> GetObjectsWithFilter(Func filter) {
		vector<GameObject*> objects;
		for (GameObject *gameObject : activeList)
			if (filter(gameObject))
				objects.push_back(gameObject);
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
	//	   else return nullptr.
	GameObject* GetArchetype(const char* name) const;

	// Returns a reference to the singleton instance of the GameObjectManager.
	//static GameObjectManager& GetInstance();
	/*static GameObjectManager *InitLayer(unsigned layer, bool updateLower, bool drawLower);
	static GameObjectManager *GetLayer(unsigned layer);
	static void DeleteLayer(unsigned layer);
	static void ShutdownLayers();*/

	Space* GetSpace();
	LevelManager* GetLevelManager();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Constructor is private to prevent accidental instantiations.
	GameObjectManager(Space *space);
	~GameObjectManager();

	// Disable copy constructor and assignment operator
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------
	/*struct Layer {
		bool updateLower;
		bool drawLower;
		GameObjectManager *instance;
	};*/

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	vector<GameObject*> activeList;
	vector<GameObject*> archetypes;

	Space *space;

	//static GameObjectManager *instance;
	//static vector<Layer> instances;
	//static Layer layers[MAX_LAYERS];

	friend class Space;
};
//------------------------------------------------------------------------------
