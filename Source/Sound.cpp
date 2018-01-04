/*******************************************************************************
Filename: Sound.cpp
Author: Mark Culp (mark.culp)
Course: GAM150
Project: GAM150 Engine
Date: 3 January 2017
Description:
Implementation file for sounds
******************************************************************************/
#include "stdafx.h"
#include "Sound.h"

Sound::Sound(const char* name, const char* path, bool doLoop)
	: _name(name), _path(path), isLooping(doLoop) {}

Sound::Sound(Sound& other)
	: _name(other._name), _path(other._path), isLooping(other.isLooping) {}

const char* Sound::GetPath() const
{
	return _path;
}

bool Sound::IsNamed(const char* cmpStr) const
{
	int count = 0;

	while (_name[count])
	{
		if (!cmpStr[count] || _name[count] != cmpStr[count])
		{
			return false;
		}

		count++;
	}

	return true;
}