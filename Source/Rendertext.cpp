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
#include "Space.h"

Text::Text() : Component("Text"), active(true), offset({ 0, 0 }) {
}

Text::Text(const Text & other) : Component("Text"),
	active(other.active), str(other.str), scale(other.scale), offset(other.offset), sprite(other.sprite ? (Sprite*)other.sprite->Clone() : nullptr)
{
}

Component* Text::Clone() const{
	return new Text(*this);
}

void Text::SetActive(bool active) {
	this->active = active;
}

void Text::SetText(const char* text) {
	str = text;
}

void Text::SetText(string text) {
	str = text;
}

void Text::SetColor(Color color) {
	sprite->SetModulateColor(color);
}

void Text::SetOffset(Vector2D theOffset)
{
	offset = theOffset;
}

bool Text::GetActive() const {
	return active;
}

string Text::GetText() const {
	return str;
}

Color Text::GetColor() const {
	return sprite->GetModulateColor();
}

Vector2D Text::GetScale() const {
	return scale;
}

Vector2D Text::GetOffset() const {
	return offset;
}

void Text::SetScale(Vector2D textScale)
{
	scale = textScale;
}

void Text::Draw(Camera *cam) const {
	if (!active) return;

	int len = str.size();
	Transform* transform = GetParent()->GetComponent<Transform>();
	Vector2D startPos = transform->GetTranslation();
	Vector2D startScale = transform->GetScale();
	transform->SetTranslation(transform->GetTranslation() + offset - Vector2D((scale.x * 0.6f * len) / 2, 0.0f));
	transform->SetScale(scale);

	//Print each character in the string.
	for (int i = 0; i < len; i++){

		if (str[i] == 32) sprite->SetFrame(0);
		else if (str[i] >= 32 && str[i] <= 96) sprite->SetFrame(str[i] - 31);
		else if (str[i] >= 97 && str[i] <= 122) sprite->SetFrame(str[i] - 63);
		else sprite->SetFrame(0);
		
		sprite->Draw(cam, *transform);

		transform->SetTranslation(transform->GetTranslation() + Vector2D(scale.x * 0.6f, 0.0f));
	}

	transform->SetTranslation(startPos);
	transform->SetScale(startScale);
}

void Text::Load(rapidjson::Value & obj)
{
	if (obj.HasMember("Active") && obj["Active"].IsFalse())
		active = false;

	sprite = new Sprite();
	sprite->SetParent(GetParent());
	sprite->Load(obj);

	str = obj["Text"].GetString();
	scale = Vector2D((float)obj["Scale"][0].GetInt(), (float)obj["Scale"][1].GetInt());

	if (obj.HasMember("Offset"))
	{
		offset = Vector2D(obj["Offset"].GetArray()[0].GetFloat(), obj["Offset"].GetArray()[1].GetFloat());
	}
}
