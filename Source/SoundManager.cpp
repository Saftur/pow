/*******************************************************************************
Filename: SoundManager.cpp
Author: Mark Culp (mark.culp)
Course: GAM150
Project: GAM150 Engine
Date: 3 January 2017
Description:
Implementation file for sound controller
******************************************************************************/

#include "stdafx.h"
#include "SoundManager.h"
#include "Trace.h"

void SoundManager::Init()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&system);

	if (result != FMOD_OK)
	{
		Trace::GetInstance().GetStream() << "FMOD system initialization error! (" << result << ") " << std::endl;
	}

	result = system->init(512, FMOD_INIT_NORMAL, 0);

	if (result != FMOD_OK)
	{
		Trace::GetInstance().GetStream() << "FMOD initialization error! (" << result << ") " << std::endl;
	}

	soundArchetypeList = SoundList();
	soundActiveList = SoundList();

	soundArchetypeList.maxSounds = soundListSize;
	soundActiveList.maxSounds = soundListSize;
}

void SoundManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void SoundManager::Shutdown()
{
	for (unsigned i = 0; i < soundActiveList.maxSounds; i++)
	{
		if (&soundActiveList.soundList[i])
		{
			delete(&soundActiveList.soundList[i]);

			soundActiveList.soundList[i] = NULL;
			soundActiveList.soundCount--;
		}
	}

	for (unsigned i = 0; i < soundArchetypeList.maxSounds; i++)
	{
		if (&soundArchetypeList.soundList[i])
		{
			delete(&soundArchetypeList.soundList[i]);

			soundArchetypeList.soundList[i] = NULL;
			soundArchetypeList.soundCount--;
		}
	}
}

FMOD::System* SoundManager::GetSystem()
{
	return system;
}

void SoundManager::AddArchetype(Sound& sound)
{
	if (soundArchetypeList.soundCount == soundArchetypeList.maxSounds)
		return;

	soundArchetypeList.soundList[soundArchetypeList.soundCount++] = &sound;
}

void SoundManager::Add(Sound& sound)
{
	if (soundActiveList.soundCount == soundActiveList.maxSounds)
		return;

	soundActiveList.soundList[soundActiveList.soundCount++] = new Sound(sound);
}

Sound* SoundManager::GetSoundByName(const char* name)
{
	for (unsigned i = 0; i < soundArchetypeList.maxSounds; i++)
	{
		if (soundActiveList.soundList[i])
		{
			if (soundActiveList.soundList[i])
			{
				if (soundActiveList.soundList[i]->IsNamed(name))
					return soundActiveList.soundList[i];
			}
		}
	}

	return NULL;
}

Sound* SoundManager::GetArchetype(const char* name)
{
	for (int i = 0; i < soundListSize; i++)
	{
		if (soundArchetypeList.soundList[i])
		{
			if (soundArchetypeList.soundList[i]->IsNamed(name))
				return soundArchetypeList.soundList[i];
		}
	}

	return NULL;
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager obj;

	return obj;
}

SoundManager::SoundManager()
{}