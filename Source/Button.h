//------------------------------------------------------------------------------
//
// File Name:	Button.h
// Author(s):	Brand Knutson & Arthur Bouvier
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

#include "Component.h"
#include "Rendertext.h"
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <string>
using std::string;

#include "BehaviorArmy.h"
#include "Building.h"

typedef class Vector2D Vector2D;
typedef class GameObject GameObject;
typedef class Transform Transform;
typedef struct AEGfxTexture AEGfxTexture;
typedef class Component Component;
typedef class SpriteSource SpriteSource;
typedef class Text Text;
typedef struct Color color;

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
	Button(const char *effectName);

	void SetEffect(const char *effectName);

	void Load(rapidjson::Value& obj);

	//template <typename T>
	static GameObject* CreateButton(const char* objName, const char* effectName, AEGfxVertexList* mesh,
		Vector2D pos = { 0.0f, 0.0f }, Vector2D scale = { 100.0f, 50.0f },
		const char* text = nullptr, Vector2D textScale = { 15, 15 }, Color color = { 0, 0, 0, 0 },
		const char* font = "Assets\\Comic Sans.png");
	/*{
		GameObject* button = new GameObject(objName);
		Transform* transform = new Transform(pos.X(), pos.Y());
		transform->SetFollowCamera(false);
		transform->SetScale(scale);
		button->AddComponent(transform);
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetMeshHalfSize({ 0.5f, 0.5f });
		sprite->SetMeshUV({ 1.0f, 1.0f });
		button->AddComponent(sprite);
		if(text) {
			Text* textObj = new Text(true, text, font, color, textScale);
			button->AddComponent((Component*)textObj);
		}

		T* buttonType = new T();
		button->AddComponent((Component*)buttonType);
		return button;
	}*/

	static void Shutdown();
	
	static void ListEffects();
	static void AddClickEffect(const char *name, void(*effectFunc)(Button&,float, int, ...), Building::BuildingType type = Building::BuildingType::Null);
	static void (*GetClickEffect(const char *name))(Button&,float, int, ...);

	static void ForceClick(Button& button, float dt, unsigned count = 0, ...);

	static map<string, Building::BuildingType> buildingType; //Type of building this is, paired with the name of the effect.
	string effectName; //The name of the effect on this building.

	bool active = true; //Is this button active.

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Clone an advanced behavior and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a dynamically allocated clone of the advanced behavior.
	virtual Component* Clone() const;

	//Check if the button is being pressed
	void Update(float dt);

	//Update anything on the button that needs updating.
	//virtual void OnUpdate(float dt);

	//What happens when a button is clicked.
	//virtual void ClickEffect(float dt) = 0;
	void(*ClickEffect)(Button& button, float dt, int count, ...);

	static map<string, void(*)(Button&,float, int, ...)> clickEffects;

	// Click effects
	static void RestartEffect(Button &button, float dt, int count = 0, ...);
	static void QuitEffect(Button &button, float dt, int count = 0, ...);
	// End click effects

	//Building Buttons
	static void CreateMineEffect(Button &button, float dt, int count = 0,  ...);

	static vector<AEGfxTexture*> textures;
};

//------------------------------------------------------------------------------
