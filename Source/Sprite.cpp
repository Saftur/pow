#include "stdafx.h"
#include "Sprite.h"
#include <AEEngine.h>
#include "GameObject.h"
#include "SpriteSource.h"
#include "Transform.h"
#include "Trace.h"
#include "LevelManager.h"
#include "Mesh.h"

Sprite::Sprite() :
		Component("Sprite"), alpha(1), color({ 0, 0, 0, 0 }), frameIndex(0)
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
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
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

void Sprite::Load(rapidjson::Value& obj)
{
	if (obj.HasMember("SpriteSource") && obj["SpriteSource"].IsString())
	{
		// Add a sprite source by name.
		spriteSource = LevelManager::GetInstance().GetSpriteSource(obj["SpriteSource"].GetString());
	}
	else if (obj.HasMember("SpriteSource"))
	{
		// Create and add a sprite source.
		SpriteSource* ss = new SpriteSource(0, 0, nullptr);

		ss->Load(obj["SpriteSource"]);
		spriteSource = ss;

		rapidjson::Value& tmp = obj["SpriteSource"];

		// Add the sprite source to the map.
		LevelManager::GetInstance().AddSpriteSource(tmp["Name"].GetString(), ss);
	}

	if (obj.HasMember("Mesh") && obj["Mesh"].IsString())
	{
		// Add a mesh by name.
		mesh = LevelManager::GetInstance().GetMesh(obj["Mesh"].GetString());
	}
	else if (obj.HasMember("Mesh"))
	{
		// Create and add a mesh.
		rapidjson::Value& tmp = obj["Mesh"];
		rapidjson::Value& tmp1 = tmp["HalfSize"];
		rapidjson::Value& tmp2 = tmp["UV"];

		mesh = MeshCreateQuad(tmp1[0].GetFloat(), tmp1[1].GetFloat(), tmp2[0].GetFloat(), tmp2[1].GetFloat());

		// Add the mesh to the map.
		LevelManager::GetInstance().AddMesh(tmp["Name"].GetString(), mesh);
	}

	if (obj.HasMember("Alpha") && obj["Alpha"].IsFloat())
		alpha = obj["Alpha"].GetFloat();
	if (obj.HasMember("FrameIndex") && obj["FrameIndex"].IsInt())
		frameIndex = obj["FrameIndex"].GetInt();

	if (obj.HasMember("ModulateColor") && obj["ModulateColor"].IsArray() && obj["ModulateColor"].Size() == 4) {
		color.r = obj["ModulateColor"][0].GetFloat();
		color.g = obj["ModulateColor"][1].GetFloat();
		color.b = obj["ModulateColor"][2].GetFloat();
		color.a = obj["ModulateColor"][3].GetFloat();
	}
}