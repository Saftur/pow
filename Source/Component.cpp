#include "stdafx.h"
#include "Component.h"
#include "Camera.h"
#include "Trace.h"

Component::Component(const char * name) : 
		Serializable(name), name(name)
{
}

Component::~Component()
{
}

Component * Component::Clone() const {
	return nullptr;
}

void Component::InstanceInit() {
}

void Component::Update(float dt)
{
	Trace::GetInstance().GetStream() << "Component " << name << ": Update" << std::endl;
	UNREFERENCED_PARAMETER(dt);
}

void Component::Draw(Camera *cam) const
{
	Trace::GetInstance().GetStream() << "Component " << name << ": Draw" << std::endl;
}

void Component::OnDestroy()
{
}

GameObject * Component::GetParent() const
{
	return parent;
}

void Component::SetParent(GameObject * object)
{
	parent = object;
}

const char * Component::GetName() const
{
	return name;
}
