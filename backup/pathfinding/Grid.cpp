#include "stdafx.h"
#include "Grid.h"
#include "BehaviorUnit.h"
#include "GameObjectManager.h"
#include "GridManager.h"

using Node = Grid::Node;
Grid Grid::Gridy = Grid(12, 8, 64, 64, 0, 28);

Grid::Grid()
	:Grid(10, 2, 10, 10)
{

}
/*Node::Node(Vector2D gridPos, bool state)
	: gridPos(gridPos), open(state), parent({ -1000, -1000 }) {
	Grid &grid = GetInstance();
	worldPos.X(grid.tileWidth * gridPos.x + grid.screenOffsetX - (grid.width * grid.tileWidth) / 2 + grid.tileWidth / 2);
	worldPos.Y(-(grid.tileHeight * gridPos.y + grid.screenOffsetY - (grid.height * grid.tileHeight) / 2 + grid.tileHeight / 2));
}

Grid::Node::Node(const Node & other)
	: gridPos(other.gridPos), worldPos(other.worldPos), hVal(other.hVal), gVal(other.gVal), open(other.open), parent(other.parent)
{
}


Grid::Node::operator Vector2D() const
{
	//gridPos = ;
	return gridPos;
}

int Grid::Node::X() const
{
	return (int)gridPos.x;
}

int Grid::Node::Y() const
{
	return (int)gridPos.y;
}

bool Grid::Node::operator==(const Node & other) const
{
	return X() == other.X() && Y() == other.Y();
}

bool Grid::Node::operator!=(const Node & other) const
{
	return X() != other.X() || Y() != other.Y();
}*/

Grid::Grid(int width, int height, int tileWidth, int tileHeight, int screenOffsetX, int screenOffsetY)
	: width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight), screenOffsetX(screenOffsetX), screenOffsetY(screenOffsetY)
{
	grid = new Node*[height];
	// Initialize the grid.
	for (int y = 0; y < height; y++)
	{
		grid[y] = new Node[width];

		for (int x = 0; x < width; x++)
		{
			grid[y][x] = Node(Vector2D((float)x, (float)y));
		}
	}
}

Grid::~Grid()
{
	for (int y = 0; y < height; y++)
		delete[] grid[y];
	delete[] grid;
}

Vector2D Grid::ConvertToWorldPoint(Node node)
{
	return node.worldPos;
}

Node* Grid::ConvertToGridPoint(Vector2D screenPos)
{
	Vector2D pos;
	pos.X(floor((screenPos.X() - screenOffsetX + ((tileWidth*width) / 2)) / tileWidth));
	pos.Y(floor((-screenPos.Y() - screenOffsetY + ((tileHeight*height) / 2)) / tileHeight));
	if (pos.X() < 0)
		pos.X(0);
	if (pos.Y() < 0)
		pos.Y(0);
	if (pos.X() >= width)
		pos.X((float)(width - 1));
	if (pos.Y() >= height)
		pos.Y((float)(height - 1));
	return &grid[(int)pos.y][(int)pos.x];
}

Node* Grid::operator[](int index)
{
	return grid[index];
}



std::vector<Node*> Grid::GetNeighbors(Node* node)
{
	// Create a vector to store our result.
	std::vector<Node*> result;

	// Find adjacent nodes.
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			// Make sure this is a valid position in the array.
			if (node->X() + i < 0 || node->X() + i >= width || node->Y() + j < 0 || node->Y() + j >= height)
				continue;

			// Skip the center node (the node whose neightbors we're finding).
			if (i == 0 && j == 0)
				continue;

			// Add this node to the result vector.
			result.push_back(&GetNode(node->X() + i, node->Y() + j));
		}
	}

	return result;
}

void Grid::SetNode(int xPos, int yPos, bool newState)
{
	grid[yPos][xPos].open = newState;
}

Node& Grid::GetNode(int x, int y)
{
	return grid[y][x];
}

GameObject* Grid::GetOccupant(int xPos, int yPos, GameObjectManager* manager)
{
	for (GameObject* obj : manager->GetObjectsByName("Unit"))
	{
		if (obj->GetComponent<BehaviorUnit>()->GetGridPos() == Vector2D((float)xPos, (float)yPos))
		{
			return obj;
		}
	}

	return nullptr;
}

Grid& Grid::GetInstance()
{
	return Gridy;
}