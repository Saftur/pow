//------------------------------------------------------------------------------
//
// File Name:	Button.h
// Author(s):	Brand Knutson
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Behavior.h"
#include <vector>

typedef class Vector2D Vector2D;
typedef class GameObject GameObject;
typedef class Transform Transform;
typedef struct AEGfxTexture AEGfxTexture;
typedef class Component Component;
typedef class SpriteSource SpriteSource;
//typedef class Text Text;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the class to be defined in BehaviorAsteroid.h
class Button : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new Button component.
	Button();

	template <typename T>
	static GameObject* CreateButton(const char* objName, AEGfxVertexList* mesh, Vector2D pos = { 0.0f, 0.0f }, Vector2D scale = { 100.0f, 50.0f }, const char* spritePath = nullptr) {
		GameObject* button = new GameObject(objName);
		Transform* transform = new Transform(pos.X(), pos.Y());
		transform->SetScale(scale);
		button->AddComponent(transform);
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		if (spritePath) {
			AEGfxTexture* texture = AEGfxTextureLoad(spritePath);
			textures.push_back(texture);
			SpriteSource* spriteSource = new SpriteSource(1, 1, texture);
			sprite->SetSpriteSource(spriteSource);
		}
		/*if(text) {
			Text* textObj = new Text(true, text, font);
			button->AddComponent((Component*)textObj);
		}
		*/
		button->AddComponent(sprite);

		T* buttonType = new T();
		button->AddComponent((Component*)buttonType);
		return button;
	}

	static void Shutdown();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a dynamically allocated clone of the advanced behavior.
	virtual Component* Clone() const = 0;

	//Check if the button is being pressed
	void Update(float dt);

	//Update anything on the button that needs updating.
	virtual void OnUpdate(float dt);

	//What happens when a button is clicked.
	virtual void ClickEffect(float dt) = 0;

	static std::vector<AEGfxTexture*> textures;
};

//------------------------------------------------------------------------------
