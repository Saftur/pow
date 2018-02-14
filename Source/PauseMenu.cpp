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
#include "SpriteSource.h" //Allow use of SpriteSource on buttons.
#include "QuitButton.h" //Allow use of quit button.

PauseMenu::PauseMenu() : Component("PauseMenu") {
	mesh = MeshCreateQuad(1.0f, 1.0f, 1.0f, 1.0f);
	background = nullptr;
}

PauseMenu::~PauseMenu() {
	Shutdown();
	AEGfxMeshFree(mesh);
}

void PauseMenu::Shutdown() {
	for (unsigned i = 0; i < buttons.size(); i++) {
		AEGfxTextureUnload(((SpriteSource*)buttons[i]->GetComponent("SpriteSource"))->GetTexture());
		delete buttons[i];
	}
	if (background) {
		delete background;
		background = nullptr;
	}

	buttons.clear();
}

void PauseMenu::Update(float dt) {
	for (unsigned i = 0; i < buttons.size(); i++) buttons[i]->Update(dt);
	if (background) background->Update(dt);

	//If the game is currently paused.
	if (((TimeSpace*)GetParent()->GetComponent("TimeSpace"))->IsPaused() && !background) {
		background = CreateBackground(0.5f);
		buttons.push_back(Button::CreateButton<QuitButton>("Quit Button", mesh, { 0.0f, 0.0f }, { 100.0f, 50.0f }));
	}
	if (!((TimeSpace*)GetParent()->GetComponent("TimeSpace"))->IsPaused() && background) Shutdown();
}

Component* PauseMenu::Clone() const {
	return new PauseMenu(*this);
}

GameObject* PauseMenu::CreateBackground(float alpha) {
	GameObject* tmpBackground = new GameObject("Background");
	
	Sprite* sprite = new Sprite();
	sprite->AdjustAlpha(-(1.0f - alpha));
	sprite->SetModulateColor({ 0.4f, 0.4f, 0.4f, 1.0f });
	sprite->SetMesh(mesh);
	tmpBackground->AddComponent(sprite);
	Transform* transform = new Transform(0.0, 0.0);
	transform->SetScale({ 1000, 1000 });
	tmpBackground->AddComponent(transform);
	return tmpBackground;
}
