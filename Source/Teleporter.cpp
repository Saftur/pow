#include "stdafx.h"
#include "Teleporter.h"
#include <AEEngine.h>
#include "GameObject.h"
#include "Transform.h"
#include "Trace.h"

void TeleporterUpdateObject(GameObject & gameObject)
{
	Transform *tr = gameObject.GetComponent<Transform>();
	if (!tr) return;

	f32 maxX = AEGfxGetWinMaxX();
	f32 minX = AEGfxGetWinMinX();
	f32 maxY = AEGfxGetWinMaxY();
	f32 minY = AEGfxGetWinMinY();
	Vector2D pos = tr->GetTranslation();
	Vector2D scale = tr->GetScale();
	
	if (pos.X() - (scale.X() / 2) > maxX)
		pos.X(minX - (scale.X() / 2));
	else if (pos.X() + (scale.X() / 2) < minX)
		pos.X(maxX + (scale.X() / 2));
	if (pos.Y() - (scale.Y() / 2) > maxY)
		pos.Y(minY - (scale.Y() / 2));
	else if (pos.Y() + (scale.Y() / 2) < minY)
		pos.Y(maxY + (scale.Y() / 2));

	tr->SetTranslation(pos);
}
