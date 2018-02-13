////------------------------------------------------------------------------------
//// File Name:	Rendertext.cpp
//// Author(s):	Brand Knutson
//// Project:		Push of War
//// Course:		Gam150
////
//// Copyright © 2017 DigiPen (USA) Corporation.
////------------------------------------------------------------------------------
//
#include "stdafx.h"
//#include "AEEngine.h"
//
//#include "Mesh.h"
//#include "Rendertext.h"
//#include <stdio.h>
//#include "GameObject.h"
//#include "Sprite.h"
//#include "Transform.h"
//#include "SpriteSource.h"
//#include "Vector2D.h"
//
//Text::Text(const char * text) : Component("Text") {
//	strcpy(string, text);
//	
//	mesh = MeshCreateQuad(1.0f, 1.0f, 0.0131578947, 0.0131578947f, "FontMesh");
//	texture = AEGfxTextureLoad("Assets\\FontSheet.png");
//	spritesource = new SpriteSource(16, 6, texture);
//
//	sprite.SetMesh(mesh);
//	sprite.SetSpriteSource(spritesource);
//}
//
//void Text::setText(const char* text) {
//	strcpy(string, text);
//}
//
//void Text::Update(float dt) {
//	int len = (int)strlen(string);
//	Transform* transform = (Transform*)GetParent()->GetComponent("Transform");
//	Vector2D startPos = transform->GetTranslation();
//	
//	//Print each character in the string.
//	for (int i = 0; i < len; i++){
//		if(std::isdigit(string[i]) sprite->SetFrame(atoi(string[i]) + 1); //Print out a number.
//		else if(std::isalpha(string[i]) && std::islower(string[i])) sprite->SetFrame(string[i] - 'a' + 11); //Print out a lower case letter.
//		else if(std::isalpha(string[i]) && std::isupper(string[i])) sprite->SetFrame(string[i] - 'A' + 37); //Print out an upper case letter.
//		else { //Print out a special character.
//			switch (string[i]){
//			case '!':
//				sprite->SetFrame(63);
//				break;
//			case '#':
//				sprite->SetFrame(64);
//				break;
//			case '$'
//				sprite->SetFrame(65);
//				break;
//			case '(':
//				sprite->SetFrame(66);
//				break;
//			case ')':
//				sprite->SetFrame(67);
//				break;
//			case ':':
//				sprite->SetFrame(68)
//				break;
//			case ';':
//				sprite->SetFrame(69)
//				break;
//			case '?':
//				sprite->SetFrame(70)
//				break;
//			case ',';
//				sprite->SetFrame(71);
//				break;
//			case '/':
//				sprite->SetFrame(72);
//				break;
//			case '.':
//				sprite->SetFrame(73);
//				break;
//			default:
//				sprite->SetFrame(0); //Space bar or an unsupported character.
//			}
//		}
//		
//		sprite->Draw(*transform);
//		transform->SetTranslation(transform->GetTranslation() + Vector2D(1.8f * transform->GetScale().X(), 0.0f));
//	}
//
//	transform->SetTranslation(startPos);
//}
