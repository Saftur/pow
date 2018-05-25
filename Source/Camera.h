#pragma once

#include "Vector2D.h"

#include "Component.h"

class Camera : public Component {
public:
	Camera();
	~Camera();

	void Load(rapidjson::Value& obj);

	void InstanceInit();

	Component *Clone() const;

	void OnDestroy();

	void SetViewport() const;

	void SetPos(Vector2D newPos);
	void SetSize(Vector2D newSize);
	void ChangePos(Vector2D delta);
	void ScaleSize(Vector2D scale);

	Vector2D GetPos() const;
	Vector2D GetSize() const;

private:
	Vector2D pos;
	Vector2D size;
};
