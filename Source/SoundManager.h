/*******************************************************************************
Filename: FMOD::SoundManager.h
Author: Mark Culp (mark.culp)
Course: GAM150
Project: GAM150 Engine
Date: 3 January 2017
Description:
Header file for sound controller
******************************************************************************/
#pragma once

#include "fmod.hpp"
#define GLOBAL_SOUND_PATH "Assets\\SFX\\"

typedef char* const Sound;

const int SoundListSize = 100;
const int MaxSoundNameLength = 100;

class SoundManager
{
public:
	// Initialize the SoundManager.
	void Init();
	
	// Update the SoundManager.
	void Update(float dt);

	// Shut down the SoundManager.
	void Shutdown();

	// Adds a sound to the sound registry.
	// Parameters:
	//		sound: the name of the sound file to add.
	//		path: the folder to look in. Defaults to the globabl sound path.
	void Add(Sound sound, const char* path = GLOBAL_SOUND_PATH);

	// Plays a sound.
	// Parameters:
	//		sound: the name of the sound file to play.
	//		loopCount: how many times to loop the sound. Default is 0, meaning no looping.
	//		volume: a sound-specific volume modifier. Default is 1.0.
	void PlaySFX(Sound sound, int loopCount = 0, float volume = 1.0f);

	// Sets the music.
	// Parameters:
	//		name: the name of the sound file to play.
	//		volume: a sound-specific volume modifier. Default is 1.0.
	//		doTransition: whether to apply a fadeout and fadein effect to allow for a
	//			smooth transition. Default is false.
	void SetMusic(Sound sound, float volume = 1.0f, bool doTransition = false);
	void StopMusic(bool doFadeout = false);

	void TogglePause();

	// Returns the current instance of the SoundManager.
	static SoundManager& GetInstance();
	
	// Returns the SoundManager's system (the FMOD object which is responsible for all of the sound handling).
	FMOD::System* GetSystem();

private:
	SoundManager();

	// Disable copy constructor and assignment operator
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	// Retrieves a sound from the sound registry.
	// Parameters:
	//		sound: the name of the sound to find.
	FMOD::Sound* getSound(Sound sound);

	struct SoundList
	{
		unsigned soundCount;
		unsigned maxSounds;

		FMOD::Sound* soundList[SoundListSize];
	};

	FMOD::Channel* musicChannel = NULL;
	SoundList soundRegistry;

	FMOD::System *system = NULL;
};
