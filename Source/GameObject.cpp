#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include <string>
#include "Trace.h"

GameObject::GameObject(const char * name) :
		name(name)
{
}

GameObject::GameObject(const GameObject & other) :
		name(other.name)
{
	for (unsigned i = 0; i < other.numComponents; i++) {
		AddComponent(other.components[i]->Clone());
	}
}

GameObject::~GameObject()
{
	for (unsigned i = 0; i < numComponents; i++) {
		delete components[i];
	}
}

void GameObject::AddComponent(Component * component)
{
	if (numComponents < maxNumComponents) {
		components[numComponents++] = component;
		component->SetParent(this);
	}
}

Component * GameObject::GetComponent(const char * name_) const
{
	for (unsigned i = 0; i < numComponents; i++) {
		Component * c = components[i];
		if (strcmp(c->GetName(), name_) == 0) {
			return c;
		}
	}
	return nullptr;
}

const char * GameObject::GetName() const
{
	return name;
}

bool GameObject::IsNamed(const char * name_) const
{
	return strcmp(name, name_) == 0;
}

bool GameObject::IsDestroyed() const
{
	return isDestroyed;
}

void GameObject::Update(float dt)
{
	Trace::GetInstance().GetStream() << "GameObject " << name << ": Update" << std::endl;
	for (unsigned i = 0; i < numComponents; i++) {
		components[i]->Update(dt);
	}
}

void GameObject::Draw()
{
	for (unsigned i = 0; i < numComponents; i++) {
		components[i]->Draw();
	}
}

void GameObject::Destroy()
{
	isDestroyed = true;
}
