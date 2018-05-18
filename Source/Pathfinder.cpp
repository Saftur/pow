#include "stdafx.h"
#include <algorithm>

#include "Pathfinder.h"

using Node = GridManager::Node;

vector<Node*> Pathfinder::FindPath(Node * start, Node * end)
{
	vector<GridManager::Node*> openNodes;
	vector<GridManager::Node*> closedNodes;

	for (int i = 0; i < GridManager::GetInstance().width; i++)
	{
		for (int j = 0; j < GridManager::GetInstance().height; j++)
		{
			GridManager::GetInstance().GetNode(i, j)->parent = nullptr;
		}
	}

	openNodes.push_back(start);

	while (!openNodes.empty())
	{
		Node* currNode = openNodes[0];

		vector<Node*>::iterator testNode = openNodes.begin();

		// Find the node with the lowest f-val (or h-val if f-vals are equal).
		for (; testNode < openNodes.end(); testNode++)
		{
			if ((*testNode)->fVal() < currNode->fVal() || ((*testNode)->fVal() == currNode->fVal() && (*testNode)->hVal < currNode->hVal))
			{
				currNode = *testNode;
			}
		}

		// Mark the current node as explored.
		closedNodes.push_back(currNode);
		openNodes.erase(std::remove(openNodes.begin(), openNodes.end(), currNode), openNodes.end());

		// Check if we've reached the target node. If so, retrace our steps and exit the loop.
		if (currNode == end)
		{
			vector<Node*> path = Retrace(end);

			if (path.size() > 0 && !end->open)
				if (!path.empty())
					path.pop_back();

			return path;
		}

		vector<GridManager::Node*> neighbors = GridManager::GetInstance().GetNeighbors(currNode);

		// Iterate through all nodes adjacent to the current node.
		for (vector<GridManager::Node*>::iterator neighbor = neighbors.begin();
			neighbor != neighbors.end(); 
			++neighbor)
		{
			// We don't need to update the costs on closed nodes, so skip them.
			if (find(closedNodes.begin(), closedNodes.end(), *neighbor) != closedNodes.end())
				continue;

			// Calculate each neighbor's new cost (Manhattan distance from start and end nodes).
			int newMoveCost = currNode->gVal + ManhattanDistance(currNode, *neighbor);

			// Update the path to the node if we've found a more efficient route, or if the node hasn't been explored yet.
			if (newMoveCost < (*neighbor)->gVal || find(openNodes.begin(), openNodes.end(), *neighbor) == openNodes.end())
			{
				(*neighbor)->gVal = newMoveCost;
				(*neighbor)->hVal = ManhattanDistance(*neighbor, end);

				(*neighbor)->parent = currNode;
			}

			// Add the neighboring nodes to the list of open nodes.
			if (find(openNodes.begin(), openNodes.end(), *neighbor) == openNodes.end() && (*neighbor)->open)
			{
				openNodes.push_back(*neighbor);
			}
		}
	}
	// Return null because we didn't find a path.
	return vector<Node*>();
}

vector<Node*> Pathfinder::Retrace(Node * end)
{
	vector<Node*> returnVal;

	while (end->parent)
	{
		returnVal.push_back(end);
		Node* tmp = end;
		end = end->parent;
		tmp->parent = nullptr;
	}

	return returnVal;
}

int Pathfinder::ManhattanDistance(Node * node1, Node * node2)
{
	Vector2D result = Vector2D(abs(node1->gridPos().x - node2->gridPos().x), abs(node1->gridPos().y - node2->gridPos().y));

	if (result.x == result.y)
		return 14;
	else
		return 10;
}
