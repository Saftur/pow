#pragma once
#include <vector>

#include "Vector2D.h"
#include "GameObject.h"

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
		void gridPos(Vector2D pos);

		bool operator==(const Node& other) const;
		bool operator!=(const Node& other) const;

	private:
		Node(Vector2D gridPos, bool state = true);
		Node(int x, int y, bool state = true);
		Node(const Node& other) = delete;
		Node();

		friend class GridManager;
	};

	vector<Node*> GetNeighbors(Node* node) const;

	void SetNode(Node* node, bool state);
	void SetNode(int x, int y, bool state);
	void SetNode(const Vector2D& pos, bool state);

	Node* GetNode(int x, int y) const;
	Node* GetNode(const Vector2D& pos) const;

	Vector2D ConvertToWorldPoint(Node* node) const;
	Vector2D ConvertToWorldPoint(int x, int y) const;
	Vector2D ConvertToWorldPoint(const Vector2D& pos) const;

	Vector2D ConvertToGridPoint(const Vector2D& pos) const;
	Vector2D ConvertToGridPoint(int x, int y) const;
	Vector2D ConvertToGridPoint(float x, float y) const;

	int GetDistanceBetween(Node* node1, Node* node2) const;
	int GetDistanceBetween(int x1, int y1, int x2, int y2) const;
	int GetDistanceBetween(const Vector2D& pos1, const Vector2D& pos2) const;

	///TODO: Templatize this to make it search for an occupant with the given component.
	GameObject* GetOccupant(Node* node) const;
	GameObject* GetOccupant(Vector2D node) const;
	GameObject* GetOccupant(int x, int y) const;

	bool IsWithinRange(Node* center, Node* target, int range) const;
	bool IsWithinRange(const Vector2D& center, const Vector2D& target, int range) const;
	bool IsWithinRange(int centerX, int centerY, int targetX, int targetY, int range) const;

	GridManager(int width, int height, int tileWidth, int tileHeight, int screenOffsetX = 0, int screenOffsetY = 0);
	~GridManager();

	static GridManager& GetInstance();

	const int width, height;
	const int tileWidth, tileHeight;
	const int offsetX, offsetY;
private:
	static GridManager gridy;

	GridManager();

	Node** grid;
};