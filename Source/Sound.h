/*******************************************************************************
Filename: Sound.h
Author: Mark Culp (mark.culp)
Course: GAM150
Project: GAM150 Engine
Date: 3 January 2017
Description:
Header file for sounds
******************************************************************************/
#pragma once

class Sound
{
public:
	Sound(const char* name, const char* path = "", bool doLoop = false);
	Sound(Sound& other);

	const char* GetName() const;
	const char* GetPath() const;

	bool IsNamed(const char* cmpStr) const;
	bool IsDestroyed() const;
	

private:
	const char* _name;
	const char* _path;

	bool isLooping;
	bool isDestroyed;
};
