#pragma once

#include <vector>
using std::vector;
#include "GameObject.h"


class PlatformManager {
public:
	struct Platform {
		Transform transform;
		float jump;
		float moveSpeed;
		vector<Vector2D> path;
		unsigned int pn;
		Vector2D dir;
	};

	static void Init();
	static void Update(float dt);
	static void Shutdown();

	static void AddPlatform(Transform transform, float jump = 0.0f, float moveSpeed = 0.0f, vector<Vector2D> path = {});
	static Platform* IsOnPlatform(GameObject *object, Vector2D *groundPosition = nullptr);;

private:
	static vector<Platform> platforms;
	static AEGfxVertexList *platformMesh;
	static Sprite *platformSprite;

	//static float scroll;
};