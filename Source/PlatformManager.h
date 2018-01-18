#pragma once

#include <vector>
using std::vector;
#include "GameObject.h"


class PlatformManager {
public:
	static void Init();

	static void Update(float dt);

	static void Shutdown();

	static void AddPlatform(Transform transform);
	static bool IsOnPlatform(GameObject *object, Vector2D *groundPosition = nullptr);

private:
	struct Platform {
		Transform transform;
	};

	static vector<Platform> platforms;
	static AEGfxVertexList *platformMesh;
	static Sprite *platformSprite;

	//static float scroll;
};