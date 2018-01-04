/*******************************************************************************
Filename: SoundManager.h
Author: Mark Culp (mark.culp)
Course: GAM150
Project: GAM150 Engine
Date: 3 January 2017
Description:
Header file for sound controller
******************************************************************************/
#pragma once

#include "Sound.h"
#include "fmod.hpp"

const int soundListSize = 100;

class SoundManager
{
public:
	void Init();
	void Update(float dt);
	void Shutdown();

	void Add(Sound& sound);
	void AddArchetype(Sound& sound);

	Sound* GetSoundByName(const char* name);
	Sound* GetArchetype(const char* name);

	static SoundManager& GetInstance();
	FMOD::System* GetSystem();

private:
	SoundManager();

	// Disable copy constructor and assignment operator
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	struct SoundList
	{
		unsigned soundCount;
		unsigned maxSounds;

		Sound* soundList[soundListSize];
	};

	SoundList soundActiveList;
	SoundList soundArchetypeList;

	FMOD::System *system = NULL;
};
