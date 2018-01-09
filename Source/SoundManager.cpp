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
#include <string>
#include "Trace.h"

// Initialize the SoundManager.
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

	soundRegistry = SoundList();

	soundRegistry.maxSounds = SoundListSize;
}

// Update the SoundManager.
void SoundManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	system->update();
}

// Shut down the SoundManager.
void SoundManager::Shutdown()
{
	for (unsigned i = 0; i < soundRegistry.maxSounds; i++)
	{
		if (&soundRegistry.soundList[i])
		{
			delete(&soundRegistry.soundList[i]);

			soundRegistry.soundList[i] = NULL;
			soundRegistry.soundCount--;
		}
	}
}

// Returns the SoundManager's system (the FMOD object which is responsible for all of the sound handling).
FMOD::System* SoundManager::GetSystem()
{
	return system;
}

// Adds a sound to the sound registry.
// Parameters:
//		name: the name of the sound file to add.
//		path: the folder to look in. Defaults to globabl sound path.
void SoundManager::Add(Sound sound, const char* path)
{
	if (soundRegistry.soundCount == soundRegistry.maxSounds)
		return;

 	std::string tmp(path);

	tmp += sound;

	system->createSound(tmp.c_str(), FMOD_DEFAULT, NULL, (&soundRegistry.soundList[soundRegistry.soundCount++]));
}

// Plays a sound.
// Parameters:
//		name: the name of the sound file to play.
//		loopCount: how many times to loop the sound. Default is 0, meaning no looping.
//		volume: a sound-specific volume modifier. Default is 1.0.
void SoundManager::PlaySFX(Sound name, int loopCount, float volume)
{
	FMOD::Channel* channel;

	system->playSound(getSound(name), NULL, false, &channel);
	channel->setVolume(volume);
	channel->setLoopCount(loopCount);
}

// Sets the music.
// Parameters:
//		name: the name of the sound file to play.
//		volume: a sound-specific volume modifier. Default is 1.0.
//		doTransition: whether to apply a fadeout and fadein effect to allow for a
//			smooth transition. Default is false.
void SoundManager::SetMusic(Sound name, float volume, bool doTransition)
{
	UNREFERENCED_PARAMETER(doTransition);
	system->playSound(getSound(name), NULL, false, &musicChannel);
	musicChannel->setVolume(volume);
	musicChannel->setLoopCount(-1);
}

// Stops the currently playing music.
// Parameters:
//		doFadeout: whether to apply a fadeout effect before stopping (music will play until end of file). Defaults to false.
void SoundManager::StopMusic(bool doFadeout)
{
	if (doFadeout)
	{
		// TODO: Add some code here.
	}
	else
	{
		musicChannel->stop();
	}
}

// Retrieves a sound from the sound registry.
// Parameters:
//		name: the name of the sound to find.
FMOD::Sound* SoundManager::getSound(Sound name)
{
	char tmp[MaxSoundNameLength] = "";

	for (int i = 0; i < SoundListSize; i++)
	{
		if (soundRegistry.soundList[i])
		{
			soundRegistry.soundList[i]->getName(tmp, MaxSoundNameLength);
			if (std::strcmp(name, tmp) == 0)
				return soundRegistry.soundList[i];
		}
	}

	return NULL;
}

// Returns the current instance of the SoundManager.
SoundManager& SoundManager::GetInstance()
{
	static SoundManager obj;

	return obj;
}

SoundManager::SoundManager()
{}