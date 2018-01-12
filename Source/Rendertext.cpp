#include "stdafx.h"
#include "AEEngine.h"

#include "Mesh.h"
#include "Rendertext.h"
#include <stdio.h>

Text::Text(const char * text, Vector2D pos, Vector2D scale) : sprite(Sprite("SpriteText")), transform(Transform(pos.X(), pos.Y())){
	strcpy(string, text);
	transform.SetScale(scale);

	mesh = MeshCreateQuad(1.0f, 1.0f, 0.065666666F, 0.165666666f, "FontMesh");
	texture = AEGfxTextureLoad("Assets\\FontSheet.png");
	spritesource = new SpriteSource(16, 6, texture);

	sprite.SetMesh(mesh);
	sprite.SetSpriteSource(spritesource);
}

void Text::setText(const char* text) {
	strcpy(string, text);
}

void Text::setLocation(Vector2D loc) {
	transform.SetTranslation(loc);
}

void Text::setScale(Vector2D scale) {
	transform.SetScale(scale);
}

Text::~Text() {
	AEGfxMeshFree(mesh);
	AEGfxTextureUnload(texture);
	delete spritesource;
}

void Text::draw() {
	int len = (int)strlen(string);
	Vector2D startPos = transform.GetTranslation();

	for (int i = 0; i < len; i++) {
		switch (string[i]) {
		case '!':
			sprite.SetFrame(1);
			break;
		case '@':
			sprite.SetFrame(3);
			break;
		case ',':
			sprite.SetFrame(12);
			break;
		case '0':
			sprite.SetFrame(16);
			break;
		case '1':
			sprite.SetFrame(17);
			break;
		case '2':
			sprite.SetFrame(18);
			break;
		case '3':
			sprite.SetFrame(19);
			break;
		case '4':
			sprite.SetFrame(20);
			break;
		case '5':
			sprite.SetFrame(21);
			break;
		case '6':
			sprite.SetFrame(22);
			break;
		case '7':
			sprite.SetFrame(23);
			break;
		case '8':
			sprite.SetFrame(24);
			break;
		case '9':
			sprite.SetFrame(25);
			break;
		case ':':
			sprite.SetFrame(26);
			break;
		case ';':
			sprite.SetFrame(27);
			break;
		case '?':
			sprite.SetFrame(31);
			break;
		case 'A':
		case 'a':
			sprite.SetFrame(33);
			break;
		case 'B':
		case 'b':
			sprite.SetFrame(34);
			break;
		case 'C':
		case 'c':
			sprite.SetFrame(35);
			break;
		case 'D':
		case 'd':
			sprite.SetFrame(36);
			break;
		case 'E':
		case 'e':
			sprite.SetFrame(37);
			break;
		case 'F':
		case 'f':
			sprite.SetFrame(38);
			break;
		case 'G':
		case 'g':
			sprite.SetFrame(39);
			break;
		case 'H':
		case 'h':
			sprite.SetFrame(40);
			break;
		case 'I':
		case 'i':
			sprite.SetFrame(41);
			break;
		case 'J':
		case 'j':
			sprite.SetFrame(42);
			break;
		case 'K':
		case 'k':
			sprite.SetFrame(43);
			break;
		case 'L':
		case 'l':
			sprite.SetFrame(44);
			break;
		case 'M':
		case 'm':
			sprite.SetFrame(45);
			break;
		case 'N':
		case 'n':
			sprite.SetFrame(46);
			break;
		case 'O':
		case 'o':
			sprite.SetFrame(47);
			break;
		case 'P':
		case 'p':
			sprite.SetFrame(48);
			break;
		case 'Q':
		case 'q':
			sprite.SetFrame(49);
			break;
		case 'R':
		case 'r':
			sprite.SetFrame(50);
			break;
		case 'S':
		case 's':
			sprite.SetFrame(51);
			break;
		case 'T':
		case 't':
			sprite.SetFrame(52);
			break;
		case 'U':
		case 'u':
			sprite.SetFrame(53);
			break;
		case 'V':
		case 'v':
			sprite.SetFrame(54);
			break;
		case 'W':
		case 'w':
			sprite.SetFrame(55);
			break;
		case 'X':
		case 'x':
			sprite.SetFrame(56);
			break;
		case 'Y':
		case 'y':
			sprite.SetFrame(57);
			break;
		case 'Z':
		case 'z':
			sprite.SetFrame(58);
			break;
		default:
			sprite.SetFrame(0);
			break;
		}
		sprite.Draw(transform);
		transform.SetTranslation(transform.GetTranslation() + Vector2D(1.8f * transform.GetScale().X(), 0.0f));
	}
	transform.SetTranslation(startPos);
}
