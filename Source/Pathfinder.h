#pragma once
#include <vector>
#include "GridManager.h"

using Node = GridManager::Node;
using std::vector;

class Pathfinder
{
public:
	static vector<Node*> FindPath(Node* start, Node* end);
private:
	static vector<Node*> Retrace(Node* end);
	static int ManhattanDistance(Node* node1, Node* node2);
};