#include "stdafx.h"
#include "GameState.h"

GameState::GameState(const char * name) :
		name(name)
{
}

GameState::~GameState()
{
}

void GameState::Load()
{
}

void GameState::Init()
{
}

void GameState::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void GameState::Shutdown()
{
}

void GameState::Unload()
{
}

const char * GameState::GetName() const
{
	return name;
}
