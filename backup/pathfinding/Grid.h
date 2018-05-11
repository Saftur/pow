#pragma once
#include <vector>
#include "Vector2D.h"
#include "GameObject.h"

class Grid
{
public:
	/*struct Node
	{
		Vector2D gridPos;
		Vector2D worldPos;
		int hVal, gVal;
		bool open;
		Vector2D parent;

		int fVal() const;

		Node(Vector2D gridPos = Vector2D(0, 0), bool state = true);
		Node(const Node& other);

		operator Vector2D() const;

		int X() const;
		int Y() const;

		bool operator==(const Node &other) const;
		bool operator!=(const Node &other) const;
	};*/

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

	Node& GetNode(int x, int y);

	// Finds the unit occupying a tile.
	// Params:
	//	xPos - the x position on the grid.
	//	yPos - the y position on the grid.
	// Returns:
	// A pointer to the unit (or building) at the specified location. NULL if unoccupied.
	GameObject* GetOccupant(int xPos, int yPos, GameObjectManager* manager);

	Vector2D ConvertToWorldPoint(Node node);
	Node* ConvertToGridPoint(Vector2D pos);

	Node* operator[](int index);

	Grid(int width, int height, int tileWidth, int tileHeight, int screenOffsetX=0, int screenOffsetY=0);
	~Grid();
	static Grid& GetInstance();
private:
	static Grid Gridy;

	const int width;
	const int height;
	const int tileWidth;
	const int tileHeight;
	const int screenOffsetX;
	const int screenOffsetY;
	
	Grid();

	Node** grid;
};