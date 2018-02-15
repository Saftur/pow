//------------------------------------------------------------------------------
// File Name:	Rendertext.cpp
// Author(s):	Brand Knutson
// Project:		Push of War
// Course:		Gam150
//
// Copyright © 2017 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"

#include "Mesh.h"
#include "Rendertext.h"
#include <stdio.h>
#include "GameObject.h"
#include "Sprite.h"
#include "Transform.h"
#include "SpriteSource.h"
#include "Vector2D.h"
#include <cctype>
#include "rapidjson.h"
#include "LevelManager.h"

Text::Text(bool manualCreation, const char* text, const char* font) : Component("Text") {
	if (manualCreation) {
		strcpy(string, text);

		mesh = MeshCreateQuad(1.0f, 1.0f, 0.065666666f, 0.165666666f);
		texture = AEGfxTextureLoad(font);
		spritesource = new SpriteSource(16, 6, texture);

		sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spritesource);
	}
}

void Text::setText(const char* text) {
	strcpy(string, text);
}

void Text::Update(float dt) {
	UNREFERENCED_PARAMETER(dt);
	int len = (int)strlen(string);
	Transform* transform = (Transform*)GetParent()->GetComponent("Transform");
	Vector2D startPos = transform->GetTranslation();
	
	//Print each character in the string.
	for (int i = 0; i < len; i++){
		if(std::isdigit(string[i])) sprite->SetFrame(atoi(&string[i]) + 1); //Print out a number.
		else if(std::isalpha(string[i]) && std::islower(string[i])) sprite->SetFrame(string[i] - 'a' + 11); //Print out a lower case letter.
		else if(std::isalpha(string[i]) && std::isupper(string[i])) sprite->SetFrame(string[i] - 'A' + 37); //Print out an upper case letter.
		else { //Print out a special character.
			switch (string[i]){
			case '!':
				sprite->SetFrame(63);
				break;
			case '#':
				sprite->SetFrame(64);
				break;
			case '$':
				sprite->SetFrame(65);
				break;
			case '(':
				sprite->SetFrame(66);
				break;
			case ')':
				sprite->SetFrame(67);
				break;
			case ':':
				sprite->SetFrame(68);
				break;
			case ';':
				sprite->SetFrame(69);
				break;
			case '?':
				sprite->SetFrame(70);
				break;
			case ',':
				sprite->SetFrame(71);
				break;
			case '/':
				sprite->SetFrame(72);
				break;
			case '.':
				sprite->SetFrame(73);
				break;
			case '=':
				sprite->SetFrame(74);
				break;
			default:
				sprite->SetFrame(0); //Space bar or an unsupported character.
			}
		}
		
		sprite->Draw(*transform);
		transform->SetTranslation(transform->GetTranslation() + Vector2D(1.8f * transform->GetScale().X(), 0.0f));
	}

	transform->SetTranslation(startPos);
}

void Text::Load(rapidjson::Value & obj)
{
	if (obj.HasMember("Texture") && obj["Texture"].GetType() == rapidjson::Type::kStringType) {
		texture = LevelManager::GetInstance().GetTexture(obj["Texture"].GetString());
	}
	else if (obj.HasMember("Texture")) {
		texture = AEGfxTextureLoad(obj["Texture"].GetString());
	}

	if (obj.HasMember("SpriteSource") && obj["SpriteSource"].GetType() == rapidjson::Type::kStringType)
	{
		// Add a sprite source by name.
		spritesource = LevelManager::GetInstance().GetSpriteSource(obj["SpriteSource"].GetString());

	}
	else if (obj.HasMember("SpriteSource"))
	{
		// Create and add a sprite source.
		spritesource = new SpriteSource(16, 6, texture);
	}

	if (obj.HasMember("Mesh") && obj["Mesh"].GetType() == rapidjson::Type::kStringType)
	{
		mesh = LevelManager::GetInstance().GetMesh(obj["Mesh"].GetString());
	}
	else if (obj.HasMember("Mesh"))
	{
		mesh = MeshCreateQuad(1.0f, 1.0f, 0.065666666f, 0.165666666f);
	}

	sprite = new Sprite();
	sprite->SetMesh(mesh);
	sprite->SetSpriteSource(spritesource);

	strcpy(string, obj["Text"].GetString());

	/*alpha = obj["alpha"].getfloat();
	frameindex = obj["frameindex"].getint();

	color color_;
	color.r = obj["modulatecolor"].getarray()[0].getfloat();
	color.g = obj["modulatecolor"].getarray()[1].getfloat();
	color.b = obj["modulatecolor"].getarray()[2].getfloat();
	color.a = obj["modulatecolor"].getarray()[3].getfloat();

	color = color_;*/
}
