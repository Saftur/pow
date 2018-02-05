#include "stdafx.h"
#include "Animation.h"
#include "GameObject.h"
#include "Sprite.h"

Animation::Animation() :
		Component("Animation")
{
}

Component * Animation::Clone() const
{
	return new Animation(*this);
}

void Animation::Play(int frameCount_, float frameDuration_, bool isLooping_)
{
	this->frameIndex = 0;
	this->frameCount = frameCount_;
	this->frameDelay = 0;
	this->frameDuration = frameDuration_;
	this->isRunning = true;
	this->isLooping = isLooping_;
	GameObject* parent_ = GetParent();
	if (parent_) {
		Sprite *sprite = (Sprite*)parent_->GetComponent("Sprite");
		if (sprite)
			sprite->SetFrame(this->frameIndex);
	}
}

void Animation::PlaySequence(AnimationSequence * sequence_)
{
	this->sequence = sequence_;
	this->frameIndex = 0;
	this->frameCount = sequence_->GetFrameCount();
	this->frameDelay = 0;
	this->frameDuration = sequence_->GetFrame(0)->frameDuration;
	this->isRunning = true;
	this->isLooping = sequence_->IsLooping();
	GameObject* parent_ = GetParent();
	if (parent_) {
		Sprite *sprite = (Sprite*)parent_->GetComponent("Sprite");
		if (sprite)
			sprite->SetFrame(sequence_->GetFrame(0)->frameIndex);
	}
}

void Animation::Update(float dt)
{
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	Sprite *sprite = (Sprite*)parent_->GetComponent("Sprite");
	if (!sprite) return;
	isDone = false;
	if (!(isRunning)) return;
	frameDelay += dt;
	if (frameDelay >= frameDuration) {
		frameDelay -= frameDuration;
		if (frameIndex + 1 < frameCount) {
			frameIndex++;
			if (sequence) {
				sprite->SetFrame(sequence->GetFrame(frameIndex)->frameIndex);
				frameDuration = sequence->GetFrame(frameIndex)->frameDuration;
			} else {
				sprite->SetFrame(frameIndex);
			}
		} else {
			frameIndex = 0;
			isDone = true;
			if (!(isLooping)) {
				isRunning = false;
			} else {
				if (sequence) {
					sprite->SetFrame(sequence->GetFrame(frameIndex)->frameIndex);
					frameDuration = sequence->GetFrame(frameIndex)->frameDuration;
				}
				else {
					sprite->SetFrame(frameIndex);
				}
			}
		}
	}
}

bool Animation::IsDone() const
{
	return isDone;
}

void Animation::TogglePause()
{
       isRunning = !isRunning;
}
