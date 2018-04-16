#include "stdafx.h"
#include "Grid.h"

using Node = Grid::Node;

Node::Node(int gridX, int gridY, Vector2D worldPos, bool highGround, bool state)
	: gridX(gridX), gridY(gridY), worldPos(worldPos), isHighGround(highGround), open(state) {}

Grid::Grid(int width, int height)
	: width(width), height(height) 
{
	// Initialize the grid.
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			// TODO: Retreive map data frpm the level's json file.
			grid[i * width + j] = new Node(i, j);
		}
	}
}

Node* Grid::GetNode(Vector2D worldPos)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (grid[Pos(i, j)]->worldPos == worldPos)
			{
				return grid[Pos(i, j)];
			}
		}
	}

	return nullptr;
}

int Grid::Pos(int xIndex, int yIndex)
{
	return xIndex * width + yIndex;
}

Node* Grid::operator[](int index)
{
	return grid[index];
}

int Node::fVal()
{
	return hVal + gVal;
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
			if (node->gridX + i < 0 || node->gridX + i > width || node->gridY + j < 0 || node->gridY + j > height)
				continue;

			// Skip the center node (the node whose neightbors we're finding).
			if (i == 0 && j == 0)
				continue;

			// Add this node to the result vector.
			result.push_back(grid[node->gridX * width + i + node->gridY + j]);
		}
	}

	return result;
}

void Grid::SetNode(int xPos, int yPos, bool newState)
{
	grid[xPos * width + yPos]->open = newState;
}

void Grid::SetHighGround(int xPos, int yPos, bool highGround)
{
	grid[xPos * width + yPos]->isHighGround = highGround;
}

Grid& Grid::GetInstance()
{
	static Grid grid;
	return grid;
}