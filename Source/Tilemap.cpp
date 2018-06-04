#include "stdafx.h"
#include "Tilemap.h"
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "Trace.h"
#include "AEEngine.h"
#include <cstdio>
#include <cstdlib>
#include "GridManager.h"

#include "System.h"

Tilemap::Tilemap(const char* spritesheetFilename, const char* tilemapFilename, const char* collisionMapFilename,
	int onScreenOffsetX, int onScreenOffsetY, int onScreenWidth, int onScreenHeight,
	int spritesheetWidth, int spritesheetHeight) :
	Component("Tilemap"),
	offsetX(onScreenOffsetX), offsetY(onScreenOffsetY), width(onScreenWidth), height(onScreenHeight),
	sprite(new Sprite()), texture(AEGfxTextureLoad(spritesheetFilename)),
	transform(new Transform(0.0f, 0.0f))
{
	ReadFiles(tilemapFilename, collisionMapFilename); // Initializes: tilemap, collisionMap, tilemapWidth, tilemapHeight
	Trace::GetInstance().GetStream() << tilemapWidth << ", " << tilemapHeight << std::endl;
	for (int y = 0; y < tilemapHeight; y++)
	{
		for (int x = 0; x < tilemapWidth; x++)
		{
			Trace::GetInstance().GetStream() << x << ", " << y << ": " << GetTile(x, y) << std::endl;
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

Tilemap::Tilemap() :
		Component("Tilemap"), offsetX(0), offsetY(0), width(0), height(0),
		tileWidth(0), tileHeight(0), tilemap(nullptr), tilemapWidth(0), tilemapHeight(0),
		collisionMap(nullptr), sprite(nullptr), spriteSource(nullptr),
		texture(nullptr), meshQuad(nullptr), transform(nullptr)
{
}

Tilemap::~Tilemap()
{
	if (tilemap) delete[] tilemap;
	if (collisionMap) delete[] collisionMap;
	if (sprite) delete sprite;
	if (spriteSource) delete spriteSource;
	if (transform) delete transform;
	if (texture) AEGfxTextureUnload(texture);
	if (meshQuad) AEGfxMeshFree(meshQuad);
}

Component * Tilemap::Clone() const
{
	Tilemap *newTilemap = new Tilemap(*this);
	if (tilemap) {
		newTilemap->tilemap = new int[tilemapWidth*tilemapHeight];
		for (int i = 0; i < tilemapWidth*tilemapHeight; i++) {
			newTilemap->tilemap[i] = tilemap[i];
		}
	}
	if (collisionMap) {
		newTilemap->collisionMap = new int[tilemapWidth*tilemapHeight];
		for (int i = 0; i < tilemapWidth*tilemapHeight; i++) {
			newTilemap->collisionMap[i] = collisionMap[i];
		}
	}
	if (transform)
		newTilemap->transform = (Transform*)transform->Clone();
	return newTilemap;
}

void Tilemap::Draw(Camera *cam) const
{
	Transform *parentTR = GetParent() ? GetParent()->GetComponent<Transform>() : nullptr;
	for (int y = 0; y < tilemapHeight; y++)
	{
		for (int x = 0; x < tilemapWidth; x++)
		{
			if (GetTile(x, y) != -1)
			{
				sprite->SetFrame(GetTile(x, y));
				Vector2D translation = { (f32)(tileWidth * x + offsetX - width / 2 + tileWidth / 2), -(f32)(tileHeight * y + offsetY - height / 2 + tileHeight / 2) };
				if (parentTR)
					translation += parentTR->GetTranslation();
				transform->SetTranslation(translation);
				sprite->Draw(cam, *transform);
			}
		}
	}
}

Vector2D Tilemap::GetPosOnMap(Vector2D screenPos, Vector2D *offsetFromTile) const
{
	Vector2D pos;
	pos.X((float)(int)((screenPos.X() - offsetX + (width / 2)) / tileWidth));
	pos.Y((float)(int)((-screenPos.Y() - offsetY + (height / 2)) / tileHeight));
	if (offsetFromTile) {
		offsetFromTile->X(screenPos.X() - (offsetX - (width / 2) + (tileWidth / 2) + (tileWidth * pos.X())));
		offsetFromTile->Y(-screenPos.Y() - -(offsetY - (height / 2) + (tileHeight / 2) + (tileHeight * pos.Y())));
	}
	if (pos.X() < 0)
		pos.X(0);
	if (pos.Y() < 0)
		pos.Y(0);
	if (pos.X() >= tilemapWidth)
		pos.X((float)(tilemapWidth - 1));
	if (pos.Y() >= tilemapHeight)
		pos.Y((float)(tilemapHeight - 1));
	return pos;
}

Vector2D Tilemap::GetPosOnScreen(Vector2D tilePos) const
{
	Vector2D pos;
	pos.X(tileWidth * tilePos.X() + offsetX - width / 2 + tileWidth / 2);
	pos.Y(-(tileHeight * tilePos.Y() + offsetY - height / 2 + tileHeight / 2));
	return pos;
}

Vector2D Tilemap::NormalizeMapPos(Vector2D tilePos) const
{
	return GetPosOnMap(GetPosOnScreen(tilePos));
}

bool Tilemap::IsMapPosOnMap(Vector2D mapPos) const
{
	return mapPos.x >= 0 && mapPos.x < tilemapWidth && mapPos.y >= 0 && mapPos.y < tilemapHeight;
}

bool Tilemap::IsScreenPosOnMap(Vector2D screenPos) const
{
	Vector2D topLeft = GetTilemapScreenTopLeft();
	Vector2D bottomRight = GetTilemapScreenBottomRight();
	return screenPos.x >= topLeft.x && screenPos.x <= bottomRight.x && screenPos.y >= bottomRight.y && screenPos.y <= topLeft.y;
}

bool Tilemap::IsObjectCollidingWithMap(Vector2D objectPosition, Vector2D objectScale) const
{
	UNREFERENCED_PARAMETER(objectPosition);
	UNREFERENCED_PARAMETER(objectScale);
	
	float sx = objectScale.X() * tileWidth;
	float sy = objectScale.Y() * tileHeight;
	float x, y;
	int tileX, tileY;
	for (int s = 0; s < 4; s++)
	{
		for (int p = -1; p <= 1; p += 2)
		{
			x = objectPosition.X();
			y = objectPosition.Y();
			if (s == 0)
			{
				x += sx / 2;
				y += sy / 4 * p;
			}
			else if (s == 1)
			{
				x -= sx / 2;
				y += sy / 4 * p;
			}
			else if (s == 2)
			{
				y += sy / 2;
				x += sx / 4 * p;
			}
			else if (s == 3)
			{
				y -= sy / 2;
				x += sx / 4 * p;
			}
			tileX = (int)((x - offsetX + (width / 2)) / tileWidth);
			tileY = (int)((-y - offsetY + (height / 2)) / tileHeight);

			Trace::GetInstance().GetStream() << tileX << ", " << tileY << std::endl;

			if (tileX < 0 || tileX >= tilemapWidth || tileY < 0 || tileY >= tilemapHeight)
				continue;

			bool collides = GetCollides(tileX, tileY);

			if (collides) return true;
		}
	}
	
	return false;
}

void Tilemap::SetOffset(int onScreenOffsetX, int onScreenOffsetY)
{
	offsetX = onScreenOffsetX;
	offsetY = onScreenOffsetY;
}

int Tilemap::GetTile(int x, int y) const
{
	return tilemap ? tilemap[y*tilemapWidth + x] : 0;
}

int Tilemap::GetCollides(int x, int y) const
{
	return collisionMap ? collisionMap[y*tilemapWidth + x] : 0;
}

int Tilemap::GetTileWidth() const
{
	return tileWidth;
}

int Tilemap::GetTileHeight() const
{
	return tileHeight;
}

Vector2D Tilemap::GetTileSize() const
{
	return Vector2D((float)tileWidth, (float)tileHeight);
}

int Tilemap::GetTilemapWidth() const
{
	return tilemapWidth;
}

int Tilemap::GetTilemapHeight() const
{
	return tilemapHeight;
}

Vector2D Tilemap::GetTilemapSize() const
{
	return Vector2D((float)tilemapWidth, (float)tilemapHeight);
}

Vector2D Tilemap::GetTilemapScreenTopLeft() const
{
	return GetPosOnScreen({ 0, 0 }) + Vector2D( -(float)tileWidth / 2, (float)tileHeight / 2);
}

Vector2D Tilemap::GetTilemapScreenBottomRight() const
{
	return GetPosOnScreen({ (float)tilemapWidth-1, (float)tilemapHeight-1 }) + Vector2D( (float)tileWidth / 2, -(float)tileHeight / 2);
}

void Tilemap::Load(rapidjson::Value & obj)
{
	// TODO Rewrite this function
	if (obj.HasMember("SpritesheetFilename") && obj["SpritesheetFilename"].IsString() &&
		obj.HasMember("TilemapFilename") && obj["TilemapFilename"].IsString() &&
		obj.HasMember("OnScreenWidth") && obj["OnScreenWidth"].IsInt() &&
		obj.HasMember("OnScreenHeight") && obj["OnScreenHeight"].IsInt() &&
		obj.HasMember("SpritesheetWidth") && obj["SpritesheetWidth"].IsInt() &&
		obj.HasMember("SpritesheetHeight") && obj["SpritesheetHeight"].IsInt()) {
		if (obj.HasMember("OnScreenOffsetX") && obj["OnScreenOffsetX"].IsInt())
			offsetX = obj["OnScreenOffsetX"].GetInt();
		else offsetX = 0;
		if (obj.HasMember("OnScreenOffsetY") && obj["OnScreenOffsetY"].IsInt())
			offsetY = obj["OnScreenOffsetY"].GetInt();
		else offsetY = 0;
		width = obj["OnScreenWidth"].GetInt();
		height = obj["OnScreenHeight"].GetInt();
		sprite = new Sprite();
		texture = AEGfxTextureLoad(obj["SpritesheetFilename"].GetString());
		transform = new Transform(0.0f, 0.0f);
		transform->SetScale({ 1.0f, 1.0f });
		if (obj.HasMember("CollisionmapFilename") && obj["CollisionmapFilename"].IsString())
			ReadFiles(obj["TilemapFilename"].GetString(), obj["CollisionmapFilename"].GetString());
		else ReadFiles(obj["TilemapFilename"].GetString(), "");
		tileWidth = obj["OnScreenWidth"].GetInt() / tilemapWidth;
		tileHeight = obj["OnScreenHeight"].GetInt() / tilemapHeight;
		meshQuad = MeshCreateQuad((float)tileWidth / 2.0f, (float)tileHeight / 2.0f, 1.0f / obj["SpritesheetWidth"].GetInt(), 1.0f / obj["SpritesheetHeight"].GetInt());
		spriteSource = new SpriteSource(obj["SpritesheetWidth"].GetInt(), obj["SpritesheetHeight"].GetInt(), texture);
		sprite->SetMesh(meshQuad);
		sprite->SetSpriteSource(spriteSource);
	}
}

void Tilemap::ReadFiles(const char* tilemapFilename, const char* collisionMapFilename)
{
	FILE *tmFile = fopen(tilemapFilename, "rt");
	FILE *cmFile = fopen(collisionMapFilename, "rt");
	tilemapWidth = 0;
	tilemapHeight = 0;
	if (!tmFile) {
		if (cmFile) fclose(cmFile);
		tilemap = nullptr;
		collisionMap = nullptr;
		return;
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
	fclose(tmFile);

	if (cmFile) {
		collisionMap = new int[tilemapHeight*tilemapWidth];
		lineNum = 0; tileNum = 0;
		tile[1] = 0;
		while ((c = (char)fgetc(cmFile)) != EOF) {
			if (c == '\n') {
				lineNum++;
				tileNum = 0;
			} else {
				tile[0] = c;
				collisionMap[lineNum*tilemapWidth + tileNum++] = atoi(tile);
			}
		}
		fclose(cmFile);
	}

	for (int i = 0; i < tilemapHeight * tilemapWidth; i++)
	{
		GridManager& manager = GridManager::GetInstance();
		GridManager::Node* node = manager.GetNode(i % tilemapWidth, i / tilemapWidth);

		if (tilemap[i] != 0)
		{
			node->open = false;
		}

		node->height = collisionMap[i];
	}
}
