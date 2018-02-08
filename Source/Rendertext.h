//------------------------------------------------------------------------------
// File Name:	Rendertext.h
// Author(s):	Brand Knutson
// Project:		Push of War
// Course:		Gam150
//
// Copyright © 2017 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#pragma once

#include "Component.h"

class Text : public Component {
public:
	Text(const char * text = "");

	void setText(const char* string);

	void Update(float dt);

private:
	char string[512];
};