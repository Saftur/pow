#include "stdafx.h"
#include <AEEngine.h>
#include "Mesh.h"
#include "PlatformManager.h"

vector<PlatformManager::Platform> PlatformManager::platforms;
AEGfxVertexList *PlatformManager::platformMesh;
Sprite *PlatformManager::platformSprite;

//float PlatformManager::scroll = 0;

void PlatformManager::Init()
{
	AEGfxVertexList* verts;

	// Start adding vertices
	AEGfxMeshStart();

	// Add some triangles
	AEGfxTriAdd(-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFF000000, 1, 0.0f,
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1);
	AEGfxTriAdd(0.5f, -0.5f, 0xFF000000, 1, 1,
		0.5f, 0.5f, 0xFF000000, 1, 0.0f,
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1);

	// Stop adding vertices
	verts = AEGfxMeshEnd();
	platformMesh = verts;//MeshCreateQuad(0.5f, 0.5f, 1, 1, "Platform Mesh");
	platformSprite = new Sprite("Platform Sprite");
	platformSprite->SetMesh(platformMesh);
}

void PlatformManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	Vector2D st = Transform::GetCamTranslation();
	Vector2D scrMin = { AEGfxGetWinMinX(), AEGfxGetWinMinY() };
	Vector2D scrMax = { AEGfxGetWinMaxX(), AEGfxGetWinMaxY() };
	Transform *pt;
	Vector2D ptrs;
	Vector2D pscl;
	for (Platform p : platforms) {
		pt = &p.transform;
		ptrs = pt->GetTranslation();
		pscl = pt->GetScale();
		if (ptrs.X() + pscl.X() / 2 > scrMin.X() && ptrs.X() - pscl.X() / 2 < scrMax.X() &&
			ptrs.Y() + pscl.Y() / 2 > scrMin.Y() && ptrs.Y() - pscl.Y() / 2 < scrMax.Y()) {
			platformSprite->Draw(*pt);
		}
	}
}

void PlatformManager::Shutdown()
{
	platforms.clear();
	AEGfxMeshFree(platformMesh);
	delete platformSprite;
}

void PlatformManager::AddPlatform(Transform transform)
{
	platforms.push_back({ transform });
}

bool PlatformManager::IsOnPlatform(GameObject * object, Vector2D * groundPosition)
{
	Transform *ot = object->GetTransform();
	if (!ot) return false;
	Vector2D otrs = ot->GetTranslation();
	Vector2D oscl = ot->GetScale();
	Transform *pt;
	Vector2D ptrs;
	Vector2D pscl;
	for (Platform p : platforms) {
		pt = &p.transform;
		ptrs = pt->GetTranslation();
		pscl = pt->GetScale();
		if ((otrs.X() - oscl.X() / 4 > ptrs.X() - pscl.X() / 2 && otrs.X() - oscl.X() / 4 < ptrs.X() + pscl.X() / 2 ||
			otrs.X() + oscl.X() / 4 > ptrs.X() - pscl.X() / 2 && otrs.X() + oscl.X() / 4 < ptrs.X() + pscl.X() / 2) &&
			otrs.Y() - oscl.Y() / 2 > ptrs.Y() - pscl.Y() / 2 && otrs.Y() - oscl.Y() / 2 < ptrs.Y() + pscl.Y() / 2 + 1) {
			if (groundPosition) {
				groundPosition->X(otrs.X());
				groundPosition->Y(ptrs.Y() + pscl.Y() / 2 + oscl.Y() / 2);
			}
			return true;
		}
	}
	return false;
}
