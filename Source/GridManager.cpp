#include "GridManager.h"
#pragma once

using Node = GridManager::Node;

GridManager GridManager::gridy = GridManager(6, 4, 128, 128, 0, 28);

GridManager::Node::Node(Vector2D gridPos, bool state)
	: x((int)gridPos.x), y((int)gridPos.y), open(state) {}

GridManager::Node::Node(int x, int y, bool state)
	: x(x), y(y), open(state) {}

GridManager::Node::Node(const Node & other)
	: x(other.x), y(other.y), hVal(other.hVal), gVal(other.gVal), open(other.open), parent(other.parent) {}

bool GridManager::Node::operator==(const Node & other) const
{
	if (x == other.x && y == other.y)
		return true;
	return false;
}

bool GridManager::Node::operator!=(const Node & other) const
{
	if (x != other.x && y != other.y)
		return true;
	return false;
}

int GridManager::Node::fVal() const
{
	return hVal + gVal;
}

Vector2D GridManager::Node::gridPos()
{
	return Vector2D((float)x, (float)y);
}

vector<Node*> GridManager::GetNeighbors(Node * node) const
{
	return vector<Node*>();
}

void GridManager::SetNode(Node * node, bool state)
{
	grid[node->x][node->y].open = state;
}

void GridManager::SetNode(int x, int y, bool state)
{
	grid[x][y].open = state;
}

void GridManager::SetNode(Vector2D pos, bool state)
{
	grid[(int)pos.x][(int)pos.y].open = state;
}

GridManager::Node * GridManager::GetNode(int x, int y) const
{
	return &grid[x][y];
}

Node * GridManager::GetNode(Vector2D pos) const
{
	return &grid[(int)pos.x][(int)pos.y];
}

Vector2D GridManager::ConvertToWorldPoint(Node * node) const
{
	return ConvertToWorldPoint(node->x, node->y);
}

Vector2D GridManager::ConvertToWorldPoint(int x, int y) const
{
	Vector2D worldPos;
	GridManager &grid = GetInstance();
	worldPos.X(grid.tileWidth * x + grid.offsetX - (grid.width * grid.tileWidth) / 2 + grid.tileWidth / 2);
	worldPos.Y(-(grid.tileHeight * y + grid.offsetY - (grid.height * grid.tileHeight) / 2 + grid.tileHeight / 2));

	return worldPos;
}

Vector2D GridManager::ConvertToWorldPoint(Vector2D pos) const
{
	return ConvertToWorldPoint((int)pos.x, (int)pos.y);
}

Vector2D GridManager::ConvertToGridPoint(Vector2D screenPos) const
{
	Vector2D pos;
	pos.X(floor((screenPos.X() - offsetX + ((tileWidth*width) / 2)) / tileWidth));
	pos.Y(floor((-screenPos.Y() - offsetY + ((tileHeight*height) / 2)) / tileHeight));
	if (pos.X() < 0)
		pos.X(0);
	if (pos.Y() < 0)
		pos.Y(0);
	if (pos.X() >= width)
		pos.X((float)(width - 1));
	if (pos.Y() >= height)
		pos.Y((float)(height - 1));
	return pos;
}

Vector2D GridManager::ConvertToGridPoint(int x, int y) const
{
	return ConvertToGridPoint(Vector2D((float)x, (float)y));
}

Vector2D GridManager::ConvertToGridPoint(float x, float y) const
{
	return ConvertToGridPoint(Vector2D(x, y));
}

GridManager::GridManager(int width, int height, int tileWidth, int tileHeight, int screenOffsetX, int screenOffsetY)
	: width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight), offsetX(screenOffsetX), offsetY(screenOffsetY) {}

GridManager::~GridManager()
{
	for (int x = 0; x < width; x++)
		delete[] grid[x];
	delete[] grid;
}

GridManager & GridManager::GetInstance()
{
	return gridy;
}
