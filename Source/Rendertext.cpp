//------------------------------------------------------------------------------
// File Name:	Rendertext.cpp
// Author(s):	Brand Knutson
// Project:		Push of War
// Course:		Gam150
//
// Copyright © 2017 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"

#include "Mesh.h"
#include "Rendertext.h"
#include <stdio.h>
#include "GameObject.h"
#include "Sprite.h"
#include "Transform.h"
#include "SpriteSource.h"
#include "Vector2D.h"

Text::Text(const char * text) : Component("Text") {
	strcpy(string, text);
}

void Text::setText(const char* text) {
	strcpy(string, text);
}

void Text::Update(float dt) {
	int len = (int)strlen(string);
	Transform* transform = (Transform*)GetParent()->GetComponent("Transform");
	Sprite* sprite = (Sprite*)GetParent()->GetComponent("Sprite");
	Vector2D startPos = transform->GetTranslation();

	for (int i = 0; i < len; i++) {
		switch (string[i]) {
		case '!':
			sprite->SetFrame(1);
			break;
		case '@':
			sprite->SetFrame(3);
			break;
		case ',':
			sprite->SetFrame(12);
			break;
		case '0':
			sprite->SetFrame(16);
			break;
		case '1':
			sprite->SetFrame(17);
			break;
		case '2':
			sprite->SetFrame(18);
			break;
		case '3':
			sprite->SetFrame(19);
			break;
		case '4':
			sprite->SetFrame(20);
			break;
		case '5':
			sprite->SetFrame(21);
			break;
		case '6':
			sprite->SetFrame(22);
			break;
		case '7':
			sprite->SetFrame(23);
			break;
		case '8':
			sprite->SetFrame(24);
			break;
		case '9':
			sprite->SetFrame(25);
			break;
		case ':':
			sprite->SetFrame(26);
			break;
		case ';':
			sprite->SetFrame(27);
			break;
		case '?':
			sprite->SetFrame(31);
			break;
		case 'A':
		case 'a':
			sprite->SetFrame(33);
			break;
		case 'B':
		case 'b':
			sprite->SetFrame(34);
			break;
		case 'C':
		case 'c':
			sprite->SetFrame(35);
			break;
		case 'D':
		case 'd':
			sprite->SetFrame(36);
			break;
		case 'E':
		case 'e':
			sprite->SetFrame(37);
			break;
		case 'F':
		case 'f':
			sprite->SetFrame(38);
			break;
		case 'G':
		case 'g':
			sprite->SetFrame(39);
			break;
		case 'H':
		case 'h':
			sprite->SetFrame(40);
			break;
		case 'I':
		case 'i':
			sprite->SetFrame(41);
			break;
		case 'J':
		case 'j':
			sprite->SetFrame(42);
			break;
		case 'K':
		case 'k':
			sprite->SetFrame(43);
			break;
		case 'L':
		case 'l':
			sprite->SetFrame(44);
			break;
		case 'M':
		case 'm':
			sprite->SetFrame(45);
			break;
		case 'N':
		case 'n':
			sprite->SetFrame(46);
			break;
		case 'O':
		case 'o':
			sprite->SetFrame(47);
			break;
		case 'P':
		case 'p':
			sprite->SetFrame(48);
			break;
		case 'Q':
		case 'q':
			sprite->SetFrame(49);
			break;
		case 'R':
		case 'r':
			sprite->SetFrame(50);
			break;
		case 'S':
		case 's':
			sprite->SetFrame(51);
			break;
		case 'T':
		case 't':
			sprite->SetFrame(52);
			break;
		case 'U':
		case 'u':
			sprite->SetFrame(53);
			break;
		case 'V':
		case 'v':
			sprite->SetFrame(54);
			break;
		case 'W':
		case 'w':
			sprite->SetFrame(55);
			break;
		case 'X':
		case 'x':
			sprite->SetFrame(56);
			break;
		case 'Y':
		case 'y':
			sprite->SetFrame(57);
			break;
		case 'Z':
		case 'z':
			sprite->SetFrame(58);
			break;
		default:
			sprite->SetFrame(0);
			break;
		}
		sprite->Draw(*transform);
		transform->SetTranslation(transform->GetTranslation() + Vector2D(1.8f * transform->GetScale().X(), 0.0f));
	}
	transform->SetTranslation(startPos);
}
