//------------------------------------------------------------------------------
//
// File Name:	PauseMenu.h
// Author(s):	Brand Knutson
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------a

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h"
#include <vector>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef class GameObject GameObject;
typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxVertexList AEGfxVertexList;
typedef class Vector2D Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface (function prototypes) declared in the header.
class PauseMenu : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	PauseMenu();

	~PauseMenu();

	// Clones the component, returning a dynamically allocated copy.
	Component* Clone() const;

	// Logic update for this component.
	void Update(float dt);

private:
	GameObject* CreateBackground(float alpha);
	
	template <typename T>
	GameObject* CreateButton(const char* objName, Vector2D pos, Vector2D scale, const char* spritePath = nullptr);

	std::vector<AEGfxTexture*> textures;
	std::vector<GameObject*> buttons;
	AEGfxVertexList* mesh;
	GameObject* background;
};

//------------------------------------------------------------------------------
