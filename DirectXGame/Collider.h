#pragma once
#include <Vector3.h>

class Collider {
private:
	float radius_ = 1.0f;

public:

	virtual void OnCollision() = 0;

	virtual Vector3 GetWorldPosition() = 0;

	void SetRadius(float radius) { radius_ = radius; }

	float GetRadius() const { return radius_; }
};