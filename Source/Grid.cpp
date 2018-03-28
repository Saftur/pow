#include "stdafx.h"
#include "Grid.h"

Grid::Node::Node(int gridX, int gridY, Vector2D worldPos, bool state)
	: gridX(gridX), gridY(gridY), worldPos(worldPos), open(state) {}

Grid::Grid(int width, int height)
	: width(width), height(height) 
{
	// Initialize the grid.
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			grid[i * width + j] = new Node(i, j);
		}
	}
}

int Grid::Node::fVal()
{
	return hVal + gVal;
}

std::vector<Grid::Node*> Grid::GetNeighbors(Node* node)
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

Grid& Grid::GetInstance()
{
	static Grid grid;
	return grid;
}