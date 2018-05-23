#pragma once


#include "Collider.h"
#include "ColliderCircle.h"
#include "Transform.h"

class ColliderBox : public Collider {
public:
	ColliderBox();
	ColliderBox(Transform transform);

	void Load(rapidjson::Value& obj);

	ColliderBox *Clone() const;
	void Draw(Camera *cam) const;
	bool IsCollidingWith(const Collider& other) const;

private:

};
