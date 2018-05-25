#include "stdafx.h"
#include "Camera.h"

#include <Windows.h>
#include <gl\GL.h>
#include <AEEngine.h>

#include "Space.h"
#include "GameObject.h"

Camera::Camera() : Component("Camera") {
}

Camera::~Camera() {
}

void Camera::Load(rapidjson::Value & obj) {
	rapidjson::Value *tmp;

	if (obj.HasMember("ScreenPos") && (tmp = &obj["ScreenPos"])->IsArray() && tmp->Size() == 2) {
		if ((*tmp)[0].IsNumber())
			pos.x = (*tmp)[0].GetFloat();
		if ((*tmp)[1].IsNumber())
			pos.y = (*tmp)[1].GetFloat();
	}
	if (obj.HasMember("ScreenSize") && (tmp = &obj["ScreenSize"])->IsArray() && tmp->Size() == 2) {
		if ((*tmp)[0].IsNumber())
			size.x = (*tmp)[0].GetFloat();
		if ((*tmp)[1].IsNumber())
			size.y = (*tmp)[1].GetFloat();
	}
}

void Camera::InstanceInit() {
	GetParent()->GetSpace()->AddCamera(this);
}

Component * Camera::Clone() const {
	return new Camera(*this);
}

void Camera::OnDestroy() {
	GetParent()->GetSpace()->DelCamera(this);
}

void Camera::SetViewport() const {
	f32 winWidth = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	f32 winHeight = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
	glViewport((GLint)(pos.x * winWidth), (GLint)(pos.y * winHeight),
			   (GLsizei)(size.x * winWidth), (GLsizei)(size.y * winHeight));
}

void Camera::SetPos(Vector2D newPos) {
	pos = newPos;
}

void Camera::SetSize(Vector2D newSize) {
	size = newSize;
}

void Camera::ChangePos(Vector2D delta) {
	pos += delta;
}

void Camera::ScaleSize(Vector2D scale) {
	size *= scale;
}

Vector2D Camera::GetPos() const {
	return pos;
}

Vector2D Camera::GetSize() const {
	return size;
}
