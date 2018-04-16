#pragma once
#include <vector>
#include "Vector2D.h"
#include "Grid.h"

using Node = Grid::Node;

class Pathfinding
{
public:
	// Finds a path from the start node to the end node.
	// Params:
	//	start - the starting node.
	//	end - the target node.
	// Returns:
	// A standard vector containing the found path (null if none was found).
	static std::vector<Node*> FindPath(Node* start, Node* end);

private:
	// Retraces the path taken to reach the end node.
	// Params:
	//  end - the end node.
	// Returns:
	// A standard vector containing the found path.
	static std::vector<Node*> RetracePath(Node* end);

	// Finds the Manhattan distance between two nodes.
	// Params:
	//	node1 - the first node.
	//	node2 - the second node.
	// Returns:
	// The Manhattan distance between the two nodes.
	static int ManhattanDist(Node node1, Node node2);
};