//------------------------------------------------------------------------------
//
// File Name:	GameObject.h
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include <vector>
using std::vector;
#include <string>
using std::string;

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class Space Space;
typedef class LevelManager LevelManager;
typedef class GameObjectManager GameObjectManager;
typedef class Component Component;
typedef class Camera Camera;

typedef class Transform Transform;
typedef class Collider Collider;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class GameObject
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new game object.
	// Params:
	//	 name = The name of the game object being created.
	GameObject(const char * name, Space *space=nullptr);

	// Clone a game object from another game object.
	// Params:
	//	 other = A reference to the object being cloned.
	GameObject(const GameObject& other);

	// Free the memory associated with a game object.
	~GameObject();

	// Adds a component to the object.
	void AddComponent(Component* component);

	// Retrieves the component with the given name if it exists.
	template <typename T>
	T *GetComponent() const {
		unsigned hash = typeid(T).hash_code();
		for (unsigned i = 0; i < numComponents; i++)
			if (typeid(*components[i]).hash_code() == hash)
				return static_cast<T*>(components[i]);
		return nullptr;
	}
	template <>
	Transform *GetComponent<Transform>() const {
		return transform;
	}
	template <>
	Collider *GetComponent<Collider>() const {
		return collider;
	}
	template <typename T>
	T *GetChildComponent() const {
		T *child;
		for (unsigned i = 0; i < numComponents; i++) {
			child = dynamic_cast<T*>(components[i]);
			if (child)
				return child;
		}
		return nullptr;
	}

	Component* GetComponent(const char* name, int number) const;
	vector<Component*> GetComponents(const char* name) const;

	// Get the name of the object.
	// Returns:
	//		A string literal containing the name of the object.	
	const char* GetName() const;

	void SetName(const char *name);

	// Check to see if the name of an object equals a given string.
	// Params:
	//   name = The string to compare to the object name.
	// Returns:
	//		True if the names match, false otherwise.
	bool IsNamed(const char* name_) const;

	// Whether the object has been marked for destruction.
	// Returns:
	//		True if the object will be destroyed, false otherwise.
	bool IsDestroyed() const;

	void SetDestroyNext();
	bool CheckDestroyNow();

	void InstanceInit();

	// Update any components attached to the game object.
	// (Hint: You will need to call Update on the Animation and Physics components.)
	// (NOTE: You must first check for valid pointers before calling these functions.)
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt);

	// Draw any visible components attached to the game object.
	// (Hint: You will need to call Draw on the Sprite component, passing it the transform component.)
	// (NOTE: You must first check for valid pointers before calling this function.) 
	void Draw(Camera *cam);

	// Mark an object for destruction.
	void Destroy();

	Space *GetSpace() const;
	GameObjectManager *GetGameObjectManager() const;
	LevelManager *GetLevelManager() const;
	//void SetLevelManager(LevelManager *lm);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Maximum number of components
	static const int maxNumComponents = 10;

	// The name of the game object.
	string name;

	// Array of components
	Component* components[maxNumComponents];

	// Number of components currently in the object
	unsigned numComponents;

	// Whether the object has been marked for destruction.
	bool isDestroyed;

	bool destroyNext;

	Space *space;

	// Shortcut Component pointers
	//  Adding a Transform or Collider will set these
	//  Getting a Transform or Collider will return these
	Transform *transform;
	Collider *collider;
};

//------------------------------------------------------------------------------
