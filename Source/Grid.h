#pragma once
#include <vector>
#include "Pathfinding.h"

class Grid
{
public:
	struct Node
	{
		int gridX, gridY;
		Vector2D worldPos;
		int hVal, gVal;
		bool open;
		Node* parent;

		int fVal();

		Node(int gridX = 0, int gridY = 0, Vector2D worldPos = Vector2D(0, 0), bool state = false);
	};

	std::vector<Node*> GetNeighbors(Node* node);
	void SetNode(int xPos, int yPos, bool newState);

	Grid(int width, int height);
	static Grid& GetInstance();
private:
	const int width;
	const int height;
	Grid();

	Node** grid;
};