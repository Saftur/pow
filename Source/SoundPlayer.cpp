#include "stdafx.h"
#include "SoundPlayer.h"
#include "SoundManager.h"

enum states { cPlayerNormal };

SoundPlayer::SoundPlayer()
	: Component("SoundPlayer")
{
}

Component * SoundPlayer::Clone() const
{
	return new SoundPlayer(*this);
}

void SoundPlayer::Update(float dt)
{
}

void SoundPlayer::Load(rapidjson::Value & obj)
{
	if (obj.HasMember("Sound"))
	{
		SoundManager::GetInstance().Add("guitar part.wav");
		SoundManager::GetInstance().PlaySFX("guitar part.wav");
		SoundManager::GetInstance().Add("Percussion.wav");
		SoundManager::GetInstance().PlaySFX("Percussion.wav");
		soundToPlay = obj["Sound"].GetString();
	}
}

