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

Tilemap::Tilemap(const char* spritesheetFilename, const char* tilemapFilename, const char* collisionMapFilename, 
	int onScreenOffsetX, int onScreenOffsetY, int onScreenWidth, int onScreenHeight) :
		offsetX(onScreenOffsetX), offsetY(onScreenOffsetY), width(onScreenWidth), height(onScreenHeight), 
		sprite(new Sprite("TilemapSprite")), texture(AEGfxTextureLoad(spritesheetFilename))
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
	meshQuad = MeshCreateQuad((float)tileWidth / 2.0f, (float)tileHeight / 2.0f, 1.0f / tilemapWidth, 1.0f / tilemapHeight, "TilemapMesh");
	spriteSource = new SpriteSource(tilemapWidth, tilemapHeight, texture);
	sprite->SetMesh(meshQuad);
	sprite->SetSpriteSource(spriteSource);
}

Tilemap::~Tilemap()
{
	delete[] tilemap;
	delete[] collisionMap;
	delete sprite;
	delete spriteSource;
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
				sprite->Draw({ (f32)(tileWidth * x + offsetX - width / 2 + tileWidth / 2), -(f32)(tileHeight * y + offsetY - height / 2 + tileHeight / 2) });
			}
		}
	}
}

bool Tilemap::isObjectCollidingWithMap(AEVec2 objectPosition, AEVec2 objectScale)
{
	UNREFERENCED_PARAMETER(objectPosition);
	UNREFERENCED_PARAMETER(objectScale);
	
	objectScale.x *= tileWidth;
	objectScale.y *= tileHeight;
	int x, y;
	AEVec2 point = { 0, 0 };
	for (int s = 0; s < 4; s++)
	{
		for (int p = -1; p <= 1; p += 2)
		{
			point = objectPosition;
			if (s == 0)
			{
				point.x += objectScale.x / 2;
				point.y += objectScale.y / 4 * p;
			}
			else if (s == 1)
			{
				point.x -= objectScale.x / 2;
				point.y += objectScale.y / 4 * p;
			}
			else if (s == 2)
			{
				point.y += objectScale.y / 2;
				point.x += objectScale.x / 4 * p;
			}
			else if (s == 3)
			{
				point.y -= objectScale.y / 2;
				point.x += objectScale.x / 4 * p;
			}
			x = (int)((point.x - offsetX + (width / 2)) / tileWidth);
			y = (int)((-point.y - offsetY + (height / 2)) / tileHeight);

			Trace::GetInstance().GetStream() << x << ", " << y << std::endl;

			if (x < 0 || x >= tilemapWidth || y < 0 || y >= tilemapHeight)
				continue;

			bool collides = getCollides(x, y);

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
