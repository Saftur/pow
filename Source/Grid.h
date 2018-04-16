#pragma once
#include <vector>
#include "Vector2D.h"
#include "GameObject.h"

class Grid
{
public:
	struct Node
	{
		int gridX, gridY;
		Vector2D worldPos;
		int hVal, gVal;
		bool open;
		bool isHighGround;
		Node* parent;

		int fVal();

		Node(int gridX, int gridY, Vector2D worldPos = Vector2D(0, 0), bool highGround = false, bool state = false);
	};

	// Finds all nodes adjacent to a given node.
	// Params:
	//	node - the node whose neighbors we're finding.
	// Returns:
	// A standard vector containing all adjacent nodes.
	std::vector<Node*> GetNeighbors(Node* node);

	// Updates a node's values.
	// Params:
	//	xPos - the node's x coordinate on the grid.
	//	yPos - the node's y coordinate on the grid.
	//	highGround - is this tile high or low ground?
	//	newState - is this node valid for pathfinding?
	void SetNode(int xPos, int yPos, bool newState);

	void SetHighGround(int xPos, int yPos, bool highGround);

	// Finds the node with the given world position.
	// Params:
	//	worldPos - the pos to find.
	// Returns:
	// A pointer to the found node. NULL if none was found.
	Node* GetNode(Vector2D worldPos);

	// Converts a given coordinate pair into a single index.
	// Params:
	//	xIndex - the x position on the grid.
	//	yIndex - the y position on the grid.
	// Returns:
	// The computed index.
	int Pos(int xIndex, int yIndex);

	// Finds the unit occupying a tile.
	// Params:
	//	xPos - the x position on the grid.
	//	yPos - the y position on the grid.
	// Returns:
	// A pointer to the unit (or building) at the specified location. NULL if unoccupied.
	GameObject* GetOccupant(int xPos, int yPos);

	Node* operator[](int index);

	Grid(int width, int height);
	static Grid& GetInstance();
private:
	const int width;
	const int height;
	Grid();

	Node** grid;
};