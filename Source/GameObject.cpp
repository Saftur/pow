#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "Space.h"
#include "LevelManager.h"
#include "Camera.h"
#include <string>
#include "Trace.h"

GameObject::GameObject(const char * name, Space *space) :
		name(name), isDestroyed(false), destroyNext(false), numComponents(0), space(space)
{
}

GameObject::GameObject(const GameObject & other) :
		name(other.name), isDestroyed(false), destroyNext(false), numComponents(0), space(other.space)
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

Component * GameObject::GetComponent(const char * name_, int number) const
{
	int counter = number;
	for (unsigned i = 0; i < numComponents; i++) {
		Component * c = components[i];
		if (strcmp(c->GetName(), name_) == 0) {
			if (counter) counter--;
			else return c;
		}
	}
	return nullptr;
}

vector<Component*> GameObject::GetComponents(const char * name_) const
{
	vector<Component*> comps;
	for (unsigned i = 0; i < numComponents; i++) {
		Component * c = components[i];
		if (strcmp(c->GetName(), name_) == 0) {
			comps.push_back(c);
		}
	}
	return comps;
}

const char * GameObject::GetName() const
{
	return name.c_str();
}

void GameObject::SetName(const char * name_)
{
	name = name_;
}

bool GameObject::IsNamed(const char * name_) const
{
	return name == string(name_);
}

bool GameObject::IsDestroyed() const
{
	return isDestroyed;
}

void GameObject::SetDestroyNext()
{
	destroyNext = true;
}

bool GameObject::CheckDestroyNow()
{
	return destroyNext;
}

void GameObject::InstanceInit() {
	for (unsigned i = 0; i < numComponents; i++) {
		components[i]->InstanceInit();
	}
}

void GameObject::Update(float dt)
{
	Trace::GetInstance().GetStream() << "GameObject " << name << ": Update" << std::endl;
	for (unsigned i = 0; i < numComponents; i++) {
		components[i]->Update(dt);
	}
}

void GameObject::Draw(Camera *cam)
{
	for (unsigned i = 0; i < numComponents; i++) {
		components[i]->Draw(cam);
	}
}

void GameObject::Destroy()
{
	isDestroyed = true;
	for (unsigned i = 0; i < numComponents; i++) {
		components[i]->OnDestroy();
	}
}

Space * GameObject::GetSpace() const {
	return space;
}

GameObjectManager * GameObject::GetGameObjectManager() const
{
	return space->GetGameObjectManager();
}

LevelManager * GameObject::GetLevelManager() const
{
	return space->GetLevelManager();
}

/*void GameObject::SetLevelManager(LevelManager * lm)
{
	levelManager = lm;
}*/
