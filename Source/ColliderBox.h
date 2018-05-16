#pragma once


#include "Collider.h"
#include "ColliderCircle.h"
#include "Transform.h"

class ColliderBox : public Collider {
public:
	ColliderBox();
	ColliderBox(Transform transform);

	ColliderBox *Clone() const;
	void Draw(Camera *cam) const;
	bool IsCollidingWith(const Collider& other) const;

private:

};
