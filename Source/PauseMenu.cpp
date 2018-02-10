//------------------------------------------------------------------------------
//
// File Name:	PauseMenu.cpp
// Author(s):	Brand Knutson
// Project:		Project 7
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "PauseMenu.h" //Header for this file.
#include "Mesh.h" //Allow creation of meshes.
#include "Trace.h" //Trace something, idk.
#include "GameObject.h" //Create GameObject.
#include "TimeSpace.h" //Check if the game is paused.
#include "Sprite.h" //Create sprites.
#include "Transform.h" //Position GameObject.
#include "GameObjectManager.h" //Add GameObject to the game.
#include "Vector2D.h" //Use Vector2D on components.
#include "QuitButton.h" //Allow use of quit button.
#include "SpriteSource.h" //Allow use of SpriteSource on buttons.
#include "LevelButton.h"

PauseMenu::PauseMenu() : Component("PauseMenu") {
	mesh = MeshCreateQuad(1.0f, 1.0f, 1.0f, 1.0f);
	background = nullptr;
}

PauseMenu::~PauseMenu() {
	for (unsigned i = 0; i < textures.size(); i++) AEGfxTextureUnload(textures[i]);
	for (unsigned i = 0; i < buttons.size(); i++) delete buttons[i];

	textures.clear();
	buttons.clear();
	AEGfxMeshFree(mesh);
}

void PauseMenu::Update(float dt) {
	for (unsigned i = 0; i < buttons.size(); i++) buttons[i]->Update(dt);
	if (background) background->Update(dt);

	//If the game is currently paused.
	if (((TimeSpace*)GetParent()->GetComponent("TimeSpace"))->IsPaused() && !background) {
		background = CreateBackground(0.5f);
		CreateButton<QuitButton>("Quit Button", { 0.0f, 0.0f }, { 100.0f, 50.0f });
		GameObject* button = CreateButton<LevelButton>("asdasdas", { 0.0f, 0.0f }, { 100.0f, 50.0f });
		((LevelButton*)button->GetComponent("Button"))->SetLevel(1);
	}
	if (!((TimeSpace*)GetParent()->GetComponent("TimeSpace"))->IsPaused() && background) {
		background->Destroy();
		background = nullptr;
	}
}

Component* PauseMenu::Clone() const {
	return new PauseMenu(*this);
}

GameObject* PauseMenu::CreateBackground(float alpha) {
	GameObject* background = new GameObject("Background");
	
	Sprite* sprite = new Sprite();
	sprite->AdjustAlpha(-(1.0f - alpha));
	sprite->SetModulateColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	sprite->SetMesh(mesh);
	background->AddComponent(sprite);
	Transform* transform = new Transform(0.0, 0.0);
	transform->SetScale({ 1000, 1000 });
	background->AddComponent(transform);
}

template <typename T>
GameObject* PauseMenu::CreateButton(const char* objName, Vector2D pos, Vector2D scale, const char* spritePath) {
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
		button->AddComponent(spriteSource);
		sprite->SetSpriteSource(spriteSource);
	}
	button->AddComponent(sprite);

	T buttonType = new T();
	button->AddComponent(buttonType);

	buttons.push_back(button);
	return button;
}
