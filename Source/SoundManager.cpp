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
#include "AEEngine.h"

// Initialize the SoundManager.
void SoundManager::Init()
{
	CheckResult(FMOD::Studio::System::create(&studioSystem));
	CheckResult(studioSystem->initialize(64, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
	soundRegistry = SoundList();

	soundRegistry.maxSounds = SoundListSize;
}

// Update the SoundManager.
void SoundManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	GetSystem()->update();
}

void SoundManager::CheckResult(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		Trace::GetInstance().GetStream() << "FMOD system initialization error! (" << result << ") " << std::endl;
	}

	//assert(!result);
}

// Shut down the SoundManager.
void SoundManager::Shutdown()
{
	for (unsigned i = 0; i < soundRegistry.soundCount; i++)
	{
		if (soundRegistry.soundList[i])
		{
			//delete(soundRegistry.soundList[i]);
			CheckResult(soundRegistry.soundList[i]->release());

			//soundRegistry.soundList[i] = NULL;
			//soundRegistry.soundCount--;
		}
	}
	soundRegistry.soundCount = 0;
	CheckResult(studioSystem->release());
}

// Returns the SoundManager's system (the FMOD object which is responsible for all of the sound handling).
FMOD::System* SoundManager::GetSystem()
{
	static FMOD::System* system;
	CheckResult(studioSystem->getLowLevelSystem(&system));

	return system;
}

FMOD::Studio::System* SoundManager::GetStudio()
{
	return studioSystem;
}

// All events start with "event:/"
static const std::string eventPrefix = "event:/";
static const std::string audioFilePath = "Data\\Assets\\SFX\\";
static const std::string bankFilePath = "";
static const int maxEventsInBank = 5;
static const int maxNameLength = 20;

// Starts an audio event with the given name.
// The event must be in one of the loaded banks.
// Params:
//	 name = The name of the event that will be started.
FMOD::Studio::EventInstance* SoundManager::PlayEvent(const std::string& name)
{
	// Construct full event path
	std::string eventPath = eventPrefix + name;

	// Find an event with the given name from the loaded banks.
	FMOD::Studio::EventDescription* description;
	CheckResult(studioSystem->getEvent(eventPath.c_str(), &description));

	// Create an instance of the event to be played.
	FMOD::Studio::EventInstance* instance;
	CheckResult(description->createInstance(&instance));

	// Start the event (plays sounds as appropriate given the default parameters settings)
	CheckResult(instance->start());

	// Destroy the event instance once it's finished.
	CheckResult(instance->release());

	// Return the event instance so the caller can use it to modify event parameters.
	return instance;
}

// Creates an FMOD sound bank
// Params:
//   filename = Name of the FMOD bank file.
void SoundManager::AddBank(const std::string& filename)
{
	// Load the FMOD bank file.
	std::string fullPath = audioFilePath + bankFilePath + filename;
	FMOD::Studio::Bank* bank;
	CheckResult(studioSystem->loadBankFile(fullPath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
	bankList.push_back(bank);

#if _DEBUG
	std::cout << "Loaded FMOD Bank \"" << filename << "\" successfully." << std::endl;
	AESysPrintf("Loaded FMOD bank \"%s\" successfully.\n", filename.c_str());

	// Determine number of events in bank
	FMOD::Studio::EventDescription* events[maxEventsInBank];
	int eventCount;
	bank->getEventList(events, maxEventsInBank, &eventCount);
	char eventPath[maxNameLength];
	int nameSize;

	// No events - no need to print any more info
	if (eventCount == 0)
	{
		std::cout << std::endl;
		return;
	}

	std::cout << "Events in bank:" << std::endl;
	AESysPrintf("Events in bank:\n");
	for (int i = 0; i < eventCount; ++i)
	{
		// Display each event
		events[i]->getPath(eventPath, maxNameLength, &nameSize);
		std::string eventName(eventPath);
		eventName = eventName.substr(eventName.find("/") + 1, eventName.size());
		std::cout << i << ": " << "\"" << eventName << "\"" << std::endl;
		AESysPrintf("%d: \"%s\"\n", i, eventName.c_str());

		// Determine number of parameters in event
		int paramCount;
		events[i]->getParameterCount(&paramCount);
		std::cout << "Parameters for event:" << std::endl;
		AESysPrintf("Parameters for event:\n");

		for (int j = 0; j < paramCount; ++j)
		{
			// Display each event parameter
			FMOD_STUDIO_PARAMETER_DESCRIPTION param;
			events[i]->getParameterByIndex(j, &param);

			std::cout << param << std::endl;
		}
	}

	std::cout << std::endl;
#endif
}

// Helper function for printing information about parameters associated with events
std::ostream& operator<<(std::ostream& os, const FMOD_STUDIO_PARAMETER_DESCRIPTION& parameter)
{
	std::string msg = "  %s (%f by default, range of %d to %d)\n";

	os << "  " << parameter.name << " (" << parameter.defaultvalue << " by default, range of "
		<< parameter.minimum << " to " << parameter.maximum << ")" << std::endl;

	AESysPrintf("  %s (%f by default, range of %d to %d)\n", parameter.name, parameter.defaultvalue, parameter.minimum, parameter.maximum);

	std::string typeInfo;

	switch (parameter.type)
	{
	case FMOD_STUDIO_PARAMETER_GAME_CONTROLLED:
		typeInfo = "Controlled via the API using Studio::EventInstance::setParameterValue.";
		break;
	case FMOD_STUDIO_PARAMETER_AUTOMATIC_DISTANCE:
		typeInfo = "Distance between the event and the listener.";
		break;
	case FMOD_STUDIO_PARAMETER_AUTOMATIC_EVENT_CONE_ANGLE:
		typeInfo = "Angle between the event's forward vector and the vector pointing from the event to the listener (0 to 180 degrees).";
		break;
	case FMOD_STUDIO_PARAMETER_AUTOMATIC_EVENT_ORIENTATION:
		typeInfo = "Horizontal angle between the event's forward vector and listener's forward vector (-180 to 180 degrees).";
		break;
	case FMOD_STUDIO_PARAMETER_AUTOMATIC_DIRECTION:
		typeInfo = "Horizontal angle between the listener's forward vector and the vector pointing from the listener to the event (-180 to 180 degrees).";
		break;
	case FMOD_STUDIO_PARAMETER_AUTOMATIC_ELEVATION:
		typeInfo = "Angle between the listener's XZ plane and the vector pointing from the listener to the event (-90 to 90 degrees).";
		break;
	case FMOD_STUDIO_PARAMETER_AUTOMATIC_LISTENER_ORIENTATION:
		typeInfo = "Horizontal angle between the listener's forward vector and the global positive Z axis (-180 to 180 degrees).";
		break;
	}

	os << "  " << typeInfo << std::endl;
	AESysPrintf(typeInfo.c_str());
	AESysPrintf("\n");

	return os;
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

	GetSystem()->createSound(tmp.c_str(), FMOD_DEFAULT, NULL, (&soundRegistry.soundList[soundRegistry.soundCount++]));
}

// Plays a sound.
// Parameters:
//		name: the name of the sound file to play.
//		loopCount: how many times to loop the sound. Default is 0, meaning no looping.
//		volume: a sound-specific volume modifier. Default is 1.0.
void SoundManager::PlaySFX(Sound name, int loopCount, float volume)
{
	FMOD::Channel* channel;

	GetSystem()->playSound(getSound(name), NULL, false, &channel);
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
	GetSystem()->playSound(getSound(name), NULL, false, &musicChannel);
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