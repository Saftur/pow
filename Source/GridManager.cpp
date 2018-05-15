#include "stdafx.h"
#include "GridManager.h"
#include "BehaviorUnit.h"
#pragma once

using Node = GridManager::Node;

GridManager GridManager::gridy = GridManager(6, 4, 128, 128, 0, 28);

GridManager::Node::Node(Vector2D gridPos, bool state)
	: x((int)gridPos.x), y((int)gridPos.y), open(state), parent(nullptr) {}

GridManager::Node::Node(int x, int y, bool state)
	: x(x), y(y), open(state), parent(nullptr) {}

GridManager::Node::Node()
{
}

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

void GridManager::Node::gridPos(Vector2D pos)
{
	x = (int)pos.x;
	y = (int)pos.y;
}

vector<Node*> GridManager::GetNeighbors(Node * node) const
{
	vector<Node*> returnVal;

	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			Vector2D checkPos = node->gridPos() + Vector2D((float)x, (float)y);

			if (checkPos.x < 0 || checkPos.y < 0 || checkPos.x >= width || checkPos.y >= height)
				continue;

			if (x == 0 && y == 0)
				continue;

			returnVal.push_back(&grid[(int)checkPos.x][(int)checkPos.y]);
		}
	}

	return returnVal;
}

void GridManager::SetNode(Node * node, bool state)
{
	grid[node->x][node->y].open = state;
}

void GridManager::SetNode(int x, int y, bool state)
{
	grid[x][y].open = state;
}

void GridManager::SetNode(const Vector2D& pos, bool state)
{
	grid[(int)pos.x][(int)pos.y].open = state;
}

GridManager::Node * GridManager::GetNode(int x, int y) const
{
	return &grid[x][y];
}

Node * GridManager::GetNode(const Vector2D& pos) const
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
	worldPos.X((float)(grid.tileWidth * x + grid.offsetX - (grid.width * grid.tileWidth) / 2 + grid.tileWidth / 2));
	worldPos.Y((float)-(grid.tileHeight * y + grid.offsetY - (grid.height * grid.tileHeight) / 2 + grid.tileHeight / 2));

	return worldPos;
}

Vector2D GridManager::ConvertToWorldPoint(const Vector2D& pos) const
{
	return ConvertToWorldPoint((int)pos.x, (int)pos.y);
}

Vector2D GridManager::ConvertToGridPoint(const Vector2D& screenPos) const
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

int GridManager::GetDistanceBetween(Node * node1, Node * node2) const
{
	return GetDistanceBetween(node1->gridPos(), node2->gridPos());
}

int GridManager::GetDistanceBetween(int x1, int y1, int x2, int y2) const
{
	return GetDistanceBetween(Vector2D((float)x1, (float)y1), Vector2D((float)x2, (float)y2));
}

int GridManager::GetDistanceBetween(const Vector2D& pos1, const Vector2D& pos2) const
{
	return (int)pos1.Distance(pos2);
}

GameObject * GridManager::GetOccupant(Node * node) const
{
	return GetOccupant(node->gridPos());
}

GameObject * GridManager::GetOccupant(Vector2D node) const
{
	for (GameObject* unit : BehaviorUnit::allUnits)
	{
		if (unit->GetComponent<BehaviorUnit>()->GetGridPos() == node)
			return unit;
	}

	return nullptr;
}

GameObject * GridManager::GetOccupant(int x, int y) const
{
	return GetOccupant(Vector2D((float)x, (float)y));
}

bool GridManager::IsWithinRange(Node * center, Node * target, int range) const
{
	return IsWithinRange(center->gridPos(), target->gridPos(), range);
}

bool GridManager::IsWithinRange(const Vector2D & center, const Vector2D & target, int range) const
{
	if (GetDistanceBetween(center, target) <= range)
		return true;
	return false;
}

bool GridManager::IsWithinRange(int centerX, int centerY, int targetX, int targetY, int range) const
{
	return IsWithinRange(Vector2D((float)centerX, (float)centerY), Vector2D((float)targetX, (float)targetY), range);
}

GridManager::GridManager(int width, int height, int tileWidth, int tileHeight, int screenOffsetX, int screenOffsetY)
	: width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight), offsetX(screenOffsetX), offsetY(screenOffsetY) 
{
	grid = new Node*[width];
	for (int x = 0; x < width; x++)
	{
		grid[x] = new Node[height];
		for (int y = 0; y < height; y++)
		{
			grid[x][y] = Node(x, y);
		}
	}
}

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
