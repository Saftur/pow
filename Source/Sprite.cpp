#include "stdafx.h"
#include "Sprite.h"
#include <AEEngine.h>
#include "GameObject.h"
#include "SpriteSource.h"
#include "Transform.h"
#include "Trace.h"

Sprite::Sprite() :
		Component("Sprite"), alpha(1), color({ 0, 0, 0, 0 })
{
}

Component * Sprite::Clone() const
{
	return new Sprite(*this);
}

void Sprite::Draw() const
{
	GameObject *parent_ = GetParent();
	if (!parent_) return;
	Transform *transform = (Transform*)parent_->GetComponent("Transform");
	if (!transform) return;

	Draw(*transform);
}

void Sprite::Draw(Transform &transform) const {
	Component::Draw();

	if (!mesh) return;

	if (spriteSource) {
		float uOffset = 0.0f, vOffset = 0.0f;
		spriteSource->GetUV(frameIndex, &uOffset, &vOffset);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(spriteSource->GetTexture(), uOffset, vOffset);
	} else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	AEGfxSetTransparency(alpha);
	AEGfxSetBlendColor(color.r, color.g, color.b, color.a);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void Sprite::AdjustAlpha(float alphaAdjust)
{
	alpha += alphaAdjust;
}

void Sprite::SetFrame(unsigned int frameIndex_)
{
	this->frameIndex = frameIndex_;
}

void Sprite::SetMesh(AEGfxVertexList * mesh_)
{
	this->mesh = mesh_;
}

void Sprite::SetSpriteSource(SpriteSource * spriteSource_)
{
	this->spriteSource = spriteSource_;
}

void Sprite::SetModulateColor(Color color_)
{
	this->color = color_;
}
