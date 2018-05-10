#include "stdafx.h"
#include "Pathfinding.h"
#include "GameObjectManager.h"
#include <list>
#include <algorithm>
#include "BehaviorTestBox.h"

using namespace std;

vector<GameObject*> Pathfinding::testNodes;

vector<Grid::Node*> Pathfinding::FindPath(Grid::Node* start, Grid::Node* end)
{
	for (GameObject* o : testNodes)
		o->Destroy();
	testNodes.clear();

	if (!end->open)
		return vector<Node*>();

	// Create lists to store open and explored nodes.
	list<Grid::Node*> openNodes;
	list<Grid::Node*> closedNodes;

	// Add our starting node to the list of open nodes.
	openNodes.push_back(start);

	// Loop until there are no more nodes to explore.
	while (openNodes.size() > 0)
	{
		// Start with the first open node.
		Grid::Node* currNode = *openNodes.begin();

		// Find the node with the lowest f-val (or h-val if f-vals are equal).
		for (Grid::Node* node : openNodes)
		{
			int nodeF = node->fVal();
			int currNodeF = currNode->fVal();

			if (nodeF < currNodeF || nodeF == currNodeF && node->hVal < currNode->hVal && node->open)
			{
				currNode = node;
			}
		}

		// Mark the current node as explored.
		closedNodes.push_back(currNode);
		openNodes.erase(std::remove(openNodes.begin(), openNodes.end(), currNode), openNodes.end());

		/*GameObject* testObj = nullptr;
		for (GameObject* obj : testNodes)
		{
			if (obj->GetComponent<BehaviorTestBox>()->GetBoundPos() == currNode->gridPos)
				testObj = obj;
		}

		if (testObj != nullptr)
			testObj->GetComponent<BehaviorTestBox>()->SetNextState(true);*/

		// Check if we've reached the target node. If so, retrace our steps and exit the loop.
		if (currNode->gridPos == end->gridPos)
		{
			return RetracePath(currNode);
		}

		std::vector<Grid::Node*> neighbors = Grid::GetInstance().GetNeighbors(currNode);

		// Iterate through all nodes adjacent to the current node.
		for (Grid::Node* neighbor : neighbors)
		{
			// We don't need to update the costs on closed nodes, so skip them.
			if (find(closedNodes.begin(), closedNodes.end(), neighbor) != closedNodes.end())
			{
				continue;
			}

			// Calculate each neighbor's new cost (Manhattan distance from start and end nodes).
			int newMoveCost = currNode->gVal + ManhattanDist(currNode, neighbor);

			// Update the path to the node if we've found a more efficient route, or if the node hasn't been explored yet.
			if (newMoveCost < neighbor->gVal || find(openNodes.begin(), openNodes.end(), neighbor) == openNodes.end())
			{
				neighbor->gVal = newMoveCost;
				neighbor->hVal = ManhattanDist(neighbor, end);

				if (currNode != neighbor)
				{
					GameObjectManager* gom = GameObjectManager::GetLayer(0);
					GameObject* node = gom->Add(*gom->GetArchetype("TestBox"));
					node->GetComponent<BehaviorTestBox>()->SetBoundPos(neighbor->gridPos);

					testNodes.push_back(node);

					neighbor->parent = currNode->gridPos;
				}
				// Add the neighboring nodes to the list of open nodes.
				if (find(openNodes.begin(), openNodes.end(), neighbor) == openNodes.end() && neighbor->open)
				{
					openNodes.push_back(neighbor);
				}
			}
		}
	}

	// Return null because we didn't find a path.
	return vector<Grid::Node*>();
}

vector<Grid::Node*> Pathfinding::RetracePath(Grid::Node* end)
{
	vector<Grid::Node*> tmp;

	while (1)
	{
		tmp.push_back(end);
		if (end->parent == Vector2D(-1000, -1000))
			break;
		end = &Grid::GetInstance().GetNode((int)end->parent.x, (int)end->parent.y);
	}

	for (Node* tmpn : tmp)
	{
		GameObjectManager* gom = GameObjectManager::GetLayer(0);
		GameObject* node = gom->Add(*gom->GetArchetype("TestBox"));
		node->GetComponent<BehaviorTestBox>()->SetBoundPos(tmpn->gridPos);

		testNodes.push_back(node);
	}

	return tmp;
}

int Pathfinding::ManhattanDist(Grid::Node* node1, Grid::Node* node2)
{
	return (int)abs((float)(node1->gridPos.X() - node2->gridPos.X())) + (int)abs((float)(node1->gridPos.Y() - node2->gridPos.Y()));
}