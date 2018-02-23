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

Text::Text(bool manualCreation, const char* text, const char* font, Color color, Vector2D textScale) : Component("Text") {
	if (manualCreation) {
		strcpy(string, text);
		scale = textScale;

		mesh = MeshCreateQuad(0.5f, 0.5f, 0.0625f, 0.16666667f);
		texture = AEGfxTextureLoad(font);
		spritesource = new SpriteSource(16, 6, texture);

		sprite = new Sprite();
		sprite->SetMesh(mesh);
		//sprite->SetMeshHalfSize({ 0.5f, 0.5f });
		//sprite->SetMeshUV({ 0.0625f, 0.16666667f });
		sprite->SetSpriteSource(spritesource);
		sprite->SetModulateColor(color);
	}
}

Component* Text::Clone() const{
	return new Text(*this);
}

void Text::SetText(const char* text) {
	strcpy(string, text);
}

void Text::SetColor(Color color) {
	sprite->SetModulateColor(color);
}

void Text::SetScale(Vector2D textScale)
{
	scale = textScale;
}

void Text::Draw() const {
	int len = (int)strlen(string);
	Transform* transform = (Transform*)GetParent()->GetComponent("Transform");
	Vector2D startPos = transform->GetTranslation();
	Vector2D startScale = transform->GetScale();
	transform->SetTranslation(transform->GetTranslation() + Vector2D(-startScale.x/2 + scale.x/2, 0));
	transform->SetScale(scale);

	
	//Print each character in the string.
	for (int i = 0; i < len; i++){
		if (string[i] >= 32 && string[i] <= 125) sprite->SetFrame(string[i] - 32);
		else sprite->SetFrame(0);
		
		sprite->Draw(*transform);

		const char c = string[i];
		if(c == 'i' || c == '!' || c == '\'' || c == '`' || c == ':' || c == ';') transform->SetTranslation(transform->GetTranslation() + Vector2D(scale.x/8, 0.0f));
		else transform->SetTranslation(transform->GetTranslation() + Vector2D(scale.x * 0.6f, 0.0f));
	}

	transform->SetTranslation(startPos);
	transform->SetScale(startScale);
}

void Text::Load(rapidjson::Value & obj)
{
	/*if (obj.HasMember("Texture") && obj["Texture"].GetType() == rapidjson::Type::kStringType) {
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
	}*/

	sprite = new Sprite();
	sprite->Load(obj);
	//sprite->SetMesh(mesh);
	//sprite->SetSpriteSource(spritesource);

	strcpy(string, obj["Text"].GetString());
	scale = Vector2D((float)obj["Scale"][0].GetInt(), (float)obj["Scale"][1].GetInt());

	/*alpha = obj["alpha"].getfloat();
	frameindex = obj["frameindex"].getint();

	color color_;
	color.r = obj["modulatecolor"].getarray()[0].getfloat();
	color.g = obj["modulatecolor"].getarray()[1].getfloat();
	color.b = obj["modulatecolor"].getarray()[2].getfloat();
	color.a = obj["modulatecolor"].getarray()[3].getfloat();

	color = color_;*/
}
