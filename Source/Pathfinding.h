#pragma once
#include <vector>
#include "Vector2D.h"
#include "Grid.h"

class Pathfinding
{
public:
	// Finds a path from the start node to the end node.
	// Params:
	//	start - the starting node.
	//	end - the target node.
	// Returns:
	// A standard vector containing the found path (null if none was found).
	std::vector<Grid::Node*> FindPath(Grid::Node* start, Grid::Node* end);

private:
	// Retraces the path taken to reach the end node.
	// Params:
	//  end - the end node.
	// Returns:
	// A standard vector containing the found path.
	std::vector<Grid::Node*> RetracePath(Grid::Node* end);

	// Finds the Manhattan distance between two nodes.
	// Params:
	//	node1 - the first node.
	//	node2 - the second node.
	// Returns:
	// The Manhattan distance between the two nodes.
	int ManhattanDist(Grid::Node node1, Grid::Node node2);
};