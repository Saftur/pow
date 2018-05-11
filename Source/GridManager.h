#pragma once
#include "stdafx.h"
#include <vector>

#include "Vector2D.h"

using std::vector;

class GridManager
{
public:
	struct Node
	{
		int x, y;
		int hVal, gVal;
		bool open;
		Node* parent;

		int fVal() const;
		Vector2D gridPos();
		
		Node(Vector2D gridPos, bool state = true);
		Node(int x, int y, bool state = true);
		Node(const Node& other);

		bool operator==(const Node& other) const;
		bool operator!=(const Node& other) const;
	};

	vector<Node*> GetNeighbors(Node* node) const;

	void SetNode(Node* node, bool state);
	void SetNode(int x, int y, bool state);
	void SetNode(Vector2D pos, bool state);

	Node* GetNode(int x, int y) const;
	Node* GetNode(Vector2D pos) const;

	Vector2D ConvertToWorldPoint(Node* node) const;
	Vector2D ConvertToWorldPoint(int x, int y) const;
	Vector2D ConvertToWorldPoint(Vector2D pos) const;

	Vector2D ConvertToGridPoint(Vector2D pos) const;
	Vector2D ConvertToGridPoint(int x, int y) const;
	Vector2D ConvertToGridPoint(float x, float y) const;

	GridManager(int width, int height, int tileWidth, int tileHeight, int screenOffsetX = 0, int screenOffsetY = 0);
	~GridManager();

	static GridManager& GetInstance();
private:
	static GridManager gridy;

	const int width, height;
	const int tileWidth, tileHeight;
	const int offsetX, offsetY;

	GridManager();

	Node** grid;
};