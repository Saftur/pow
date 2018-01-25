#pragma once

#include <vector>
using std::vector;
#include "GameObject.h"


class PlatformManager {
public:
	struct Platform {
		Transform transform;
		float jump;
		bool trampoline;
		float moveSpeed;
		vector<Vector2D> path;
		bool killsInTime;
		unsigned int pn;
		Vector2D dir;
	};

	static void Init();
	static void Update(float dt);
	static void Shutdown();

	static void AddPlatform(Transform transform, float jump = 0.0f, bool trampoline = false, float moveSpeed = 0.0f, vector<Vector2D> path = {}, bool killsInTime = true);
	static Platform* IsOnPlatform(GameObject *object, Vector2D *groundPosition = nullptr);
	// 0 = none, 1 = right, 2 = bottom, 3 = left, 4 = top
	static int HittingSide(GameObject *object, Vector2D *newPosition = nullptr, Vector2D *newVelocity = nullptr);

private:
	static vector<Platform> platforms;
	static AEGfxVertexList *platformMesh;
	static Sprite *platformSprite;

	//static float scroll;
};