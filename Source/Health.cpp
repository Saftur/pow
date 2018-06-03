//------------------------------------------------------------------------------
//
// File Name:	ComponentHealth.cpp
// Author(s):	Arthur Bouvier
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Health.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Transform.h"
#include "LevelManager.h"

//------------------------------------------------------------------------------
// Enums:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------
Color Health::background = { 0.5f, 0.5f, 0.5f, 1.f };
Sprite* Health::bar = new Sprite();
unsigned Health::instances = 0;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Allocate a new (ComponentHealth) behavior component.
// Params:
//  parent = The object that owns this behavior.
Health::Health() : Component("Health")
{
}

void Health::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	hpPercentage = (float)hp / (float)maxHP;
	currColor = { (1.f - hpPercentage) * 2.f, hpPercentage * 2.f, 0.f, 1.f };
}

void Health::Draw(Camera* cam) const
{
	Transform barTrans = Transform();
	barTrans.SetScale(scale);
	barTrans.SetTranslation(GetParent()->GetComponent<Transform>()->GetTranslation() + offset);

	bar->SetModulateColor(background);
	bar->Draw(cam, barTrans);

	barTrans.SetScale(scale * Vector2D(hpPercentage, 1.f));
	
	bar->SetModulateColor(currColor);
	bar->Draw(cam, barTrans);
}

void Health::Load(rapidjson::Value & obj)
{
	assert(obj.HasMember("MaxHP"));

	maxHP = obj["MaxHP"].GetInt();
	hp = maxHP;

	if (obj.HasMember("Offset"))
		offset = { (float)obj["Offset"].GetArray()[0].GetInt(), (float)obj["Offset"].GetArray()[1].GetInt() };
	else
		offset = { 0, 0 };

	scale = Vector2D(10.f, 1.f) * GetParent()->GetComponent<Transform>()->GetScale() / 10.f;
}

void Health::Initialize(const int & max)
{
	maxHP = max;
	hp = max;
}

int Health::GetHP() const
{
	return hp;
}

void Health::UpdateHP(const int& amount)
{
	hp += amount;

	if (hp > maxHP)
		hp = maxHP;
	else if (hp < 0)
		GetParent()->Destroy();
}

void Health::InstanceInit()
{
	if (instances == 0)
	{
		bar->SetMesh(GetParent()->GetLevelManager()->GetMesh("ButtonMesh"));
	}

	instances++;
}

unsigned Health::GetMaxHP() const
{
	return maxHP;
}

Vector2D Health::GetOffset() const
{
	return offset;
}

void Health::SetOffset(Vector2D& newOffset)
{
	offset = newOffset;
}


// Clone an advanced behavior and return a pointer to the cloned object.
// Params:
//   behavior = Reference to the behavior that will be destroyed.
//   parent = A reference to the parent object (the object that owns this component).
// Returns:
//   A pointer to an advanced behavior.
Component* Health::Clone() const
{
	return new Health(*this);
}

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
