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
#include "Sprite.h" //Create sprites.
#include "Transform.h" //Position GameObject.
#include "GameObjectManager.h" //Add GameObject to the game.
#include "Vector2D.h" //Use Vector2D on components.
#include "SpriteSource.h" //Allow use of SpriteSource on buttons.
#include "QuitButton.h" //Allow use of quit button.
#include "Engine.h"

PauseMenu::PauseMenu() {
	
}

void PauseMenu::Init() {
	mesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f);
	background = nullptr;
}

void PauseMenu::Shutdown(bool done) {
	for (unsigned i = 0; i < buttons.size(); i++) delete buttons[i];
	if (background) {
		delete background;
		background = nullptr;
	}

	buttons.clear();
	if(done) AEGfxMeshFree(mesh);
}

void PauseMenu::Update(float dt) {
	if (background) {
		background->Update(dt);
		background->Draw();
	}
	for (unsigned i = 0; i < buttons.size(); i++) {
		buttons[i]->Update(dt);
		buttons[i]->Draw();
	}

	//If the game is currently paused.
	if (Engine::GetInstance().IsPaused() && !background) {
		background = CreateBackground(0.5f);
		buttons.push_back(Button::CreateButton<QuitButton>("Quit Button", mesh, { 0.0f, -200.0f },
			{ 200.0f, 50.0f }, "Quit Game!", {36, 40}, { 0, 0, 0, 1 }));
		buttons.push_back(Button::CreateButton<QuitButton>("Quit Button2", mesh, { 0.0f, -100.0f },
		{ 200.0f, 50.0f }, "(Probably) a Quit Button?", { 20, 24 }, { 1, 51/255, 0, 1 }));
	}
	if (!Engine::GetInstance().IsPaused() && background) Shutdown(false);
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

PauseMenu & PauseMenu::GetInstance()
{
	static PauseMenu instance;
	return instance;
}
