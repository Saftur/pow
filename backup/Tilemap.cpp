/*******************************************************************************************/
//Authors: Adam Doolittle (a.doolittle) & Arthur Bouvier (abouvier)
//FileName: TileMap.cpp
//Date: 10/17/17
//Brief: File for reading and printing the Tilemap 
/*******************************************************************************************/

#include "stdafx.h"
#include "Tilemap.h"
#include "Trace.h"
#include <cstdio>
#include <cstdlib>

#include "System.h"

Tilemap::Tilemap(const char* spritesheetFilename, const char* tilemapFilename, const char* collisionMapFilename, 
	int onScreenOffsetX, int onScreenOffsetY, int onScreenWidth, int onScreenHeight, 
		int spritesheetWidth, int spritesheetHeight) :
		offsetX(onScreenOffsetX), offsetY(onScreenOffsetY), width(onScreenWidth), height(onScreenHeight), 
		sprite(new Sprite()), texture(AEGfxTextureLoad(spritesheetFilename)), 
		transform(new Transform(0.0f, 0.0f))
{
	readFiles(tilemapFilename, collisionMapFilename); // Initializes: tilemap, collisionMap, tilemapWidth, tilemapHeight
	Trace::GetInstance().GetStream() << tilemapWidth << ", " << tilemapHeight << std::endl;
	for (int y = 0; y < tilemapHeight; y++)
	{
		for (int x = 0; x < tilemapWidth; x++)
		{
			Trace::GetInstance().GetStream() << x << ", " << y << ": " << getTilenum(x, y) << std::endl;
		}
	}
	tileWidth = onScreenWidth / tilemapWidth;
	tileHeight = onScreenHeight / tilemapHeight;
	Trace::GetInstance().GetStream() << tileWidth << ", " << tileHeight << std::endl;
	meshQuad = MeshCreateQuad((float)tileWidth / 2.0f, (float)tileHeight / 2.0f, 1.0f / spritesheetWidth, 1.0f / spritesheetHeight);
	spriteSource = new SpriteSource(spritesheetWidth, spritesheetHeight, texture);
	sprite->SetMesh(meshQuad);
	sprite->SetSpriteSource(spriteSource);
}

Tilemap::~Tilemap()
{
	delete[] tilemap;
	delete[] collisionMap;
	delete sprite;
	delete spriteSource;
	delete transform;
	AEGfxTextureUnload(texture);
	AEGfxMeshFree(meshQuad);
}

void Tilemap::Draw()
{
	for (int y = 0; y < tilemapHeight; y++)
	{
		for (int x = 0; x < tilemapWidth; x++)
		{
			if (getTilenum(x, y) != -1)
			{
				sprite->SetFrame(getTilenum(x, y));
				//sprite->Draw({ (f32)(tileWidth * x + offsetX - width / 2 + tileWidth / 2), -(f32)(tileHeight * y + offsetY - height / 2 + tileHeight / 2) });
				transform->SetTranslation({ (f32)(tileWidth * x + offsetX - width / 2 + tileWidth / 2), -(f32)(tileHeight * y + offsetY - height / 2 + tileHeight / 2) });
				sprite->Draw(*transform);
			}
		}
	}
}

Vector2D Tilemap::getPosOnMap(Vector2D screenPos, Vector2D *offsetFromTile)
{
	Vector2D pos;
	pos.X((float)(int)((screenPos.X() - offsetX + (width / 2)) / tileWidth));
	pos.Y((float)(int)((-screenPos.Y() - offsetY + (height / 2)) / tileHeight));
	if (offsetFromTile) {
		offsetFromTile->X(screenPos.X() - (offsetX - (width / 2) + (tileWidth / 2) + (tileWidth * pos.X())));
		offsetFromTile->Y(-screenPos.Y() - -(offsetY - (height / 2) + (tileHeight / 2) + (tileHeight * pos.Y())));
	}
	return pos;
}

Vector2D Tilemap::getPosOnScreen(Vector2D tilePos)
{
	Vector2D pos;
	pos.X(tileWidth * tilePos.X() + offsetX - width / 2 + tileWidth / 2);
	pos.Y(-(tileHeight * tilePos.Y() + offsetY - height / 2 + tileHeight / 2));
	return pos;
}

bool Tilemap::isObjectCollidingWithMap(Vector2D objectPosition, Vector2D objectScale)
{
	UNREFERENCED_PARAMETER(objectPosition);
	UNREFERENCED_PARAMETER(objectScale);
	
	float sx = objectScale.X() * tileWidth;
	float sy = objectScale.Y() * tileHeight;
	float x, y;
	int tileX, tileY;
	//Vector2D point = { 0, 0 };
	for (int s = 0; s < 4; s++)
	{
		for (int p = -1; p <= 1; p += 2)
		{
			//point = objectPosition;
			x = objectPosition.X();
			y = objectPosition.Y();
			if (s == 0)
			{
				//point.X(point.X() + sx / 2);
				//point.Y(point.Y() + sx / 4 * p);
				x += sx / 2;
				y += sy / 4 * p;
			}
			else if (s == 1)
			{
				//point.x -= objectScale.x / 2;
				//point.y += objectScale.y / 4 * p;
				x -= sx / 2;
				y += sy / 4 * p;
			}
			else if (s == 2)
			{
				//point.y += objectScale.y / 2;
				//point.x += objectScale.x / 4 * p;
				y += sy / 2;
				x += sx / 4 * p;
			}
			else if (s == 3)
			{
				//point.y -= objectScale.y / 2;
				//point.x += objectScale.x / 4 * p;
				y -= sy / 2;
				x += sx / 4 * p;
			}
			tileX = (int)((x - offsetX + (width / 2)) / tileWidth);
			tileY = (int)((-y - offsetY + (height / 2)) / tileHeight);

			Trace::GetInstance().GetStream() << tileX << ", " << tileY << std::endl;

			if (tileX < 0 || tileX >= tilemapWidth || tileY < 0 || tileY >= tilemapHeight)
				continue;

			bool collides = getCollides(tileX, tileY);

			if (collides) return true;
		}
	}
	
	return false;
}

void Tilemap::setOffset(int onScreenOffsetX, int onScreenOffsetY)
{
	offsetX = onScreenOffsetX;
	offsetY = onScreenOffsetY;
}

int Tilemap::getTileWidth()
{
	return tileWidth;
}

int Tilemap::getTileHeight()
{
	return tileHeight;
}

int Tilemap::getTilemapWidth()
{
	return tilemapWidth;
}

int Tilemap::getTilemapHeight()
{
	return tilemapHeight;
}

void Tilemap::readFiles(const char* tilemapFilename, const char* collisionMapFilename)
{
	FILE *tmFile = fopen(tilemapFilename, "rt");
	FILE *cmFile = fopen(collisionMapFilename, "rt");
	tilemapWidth = 0;
	tilemapHeight = 0;
	if (!tmFile || !cmFile) {
		if (tmFile) fclose(tmFile);
		if (cmFile) fclose(cmFile);
		tilemap = nullptr;
		collisionMap = nullptr;
	}
	char c;
	int lineLength = 0;
	while (true) {
		c = (char)fgetc(tmFile);
		if (c == '\n' || c == EOF) {
			if (lineLength > 0) {
				tilemapHeight++;
				lineLength = (lineLength + 1) / 3;
				tilemapWidth = (tilemapWidth == 0) ? lineLength : (lineLength < tilemapWidth ? lineLength : tilemapWidth);
				lineLength = 0;
			}
			if (c == EOF)
				break;
		} else {
			lineLength++;
		}
	}
	fseek(tmFile, 0, SEEK_SET);
	tilemap = new int[tilemapHeight*tilemapWidth];
	collisionMap = new bool[tilemapHeight*tilemapWidth];
	int lineNum = 0, tileNum = 0;
	char tile[3];
	tile[2] = 0;
	while (true) {
		tile[0] = (char)fgetc(tmFile);
		Trace::GetInstance().GetStream() << tile[0] << std::endl;
		if (tile[0] == '\n') {
			lineNum++;
			tileNum = 0;
			continue;
		} else if (tile[0] == EOF) {
			break;
		}
		tile[1] = (char)fgetc(tmFile);
		if (tile[0] == ' ') {
			tile[0] = tile[1];
			tile[1] = (char)fgetc(tmFile);
		}
		Trace::GetInstance().GetStream() << lineNum << ", " << tileNum << ":" << lineNum*tilemapWidth + tileNum << ": " << strtol(tile, nullptr, 16) << std::endl;
		tilemap[lineNum*tilemapWidth + tileNum++] = (tile[0] == ' ' && tile[1] == ' ') ? -1 : strtol(tile, nullptr, 16);
	}
	lineNum = 0; tileNum = 0;
	tile[1] = 0;
	while ((c = (char)fgetc(cmFile)) != EOF) {
		if (c == '\n') {
			lineNum++;
			tileNum = 0;
		}  else {
			tile[0] = c;
			collisionMap[lineNum*tilemapWidth + tileNum++] = atoi(tile) != 0;
		}
	}
	fclose(tmFile);
	fclose(cmFile);
}

int Tilemap::getTilenum(int x, int y)
{
	return tilemap[y*tilemapWidth + x];
}

int Tilemap::getCollides(int x, int y)
{
	return collisionMap[y*tilemapWidth + x];
}
