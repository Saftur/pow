#include "stdafx.h"
#include "Behavior.h"
#include "Component.h"

Behavior::Behavior() :
		Component("Behavior"), stateCurr(cBehaviorInvalid)
{
}

Behavior::~Behavior()
{
}

void Behavior::Update(float dt)
{
	if (stateCurr != stateNext) {
		OnExit();
		stateCurr = stateNext;
		OnEnter();
	}
	OnUpdate(dt);
}

void Behavior::OnEnter()
{
}

void Behavior::OnUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void Behavior::OnExit()
{
}

int Behavior::GetCurrentState() const
{
	return stateCurr;
}

void Behavior::SetNextState(int state)
{
	this->stateNext = state;
}

float Behavior::GetTimer() const
{
	return timer;
}

void Behavior::SetTimer(float time)
{
	this->timer = time;
}

void Behavior::SetCurrentState(int state)
{
	this->stateCurr = state;
}
