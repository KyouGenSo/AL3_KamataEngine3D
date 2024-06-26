#pragma once
#include <Vector3.h>
#include "CollisionConfig.h"

class Collider {
private:
	float radius_ = 1.0f;

	uint32_t collisionSide_ = 0xffffffff;

	uint32_t collisionMask_ = 0xffffffff;

public:
	virtual void OnCollision() = 0;

	// getter
	virtual Vector3 GetWorldPosition() = 0;

	float GetRadius() const { return radius_; }

	uint32_t GetCollisionSide() const { return collisionSide_; }

	uint32_t GetCollisionMask() const { return collisionMask_; }

	// setter
	void SetRadius(float radius) { radius_ = radius; }

	void SetCollisionSide(uint32_t side) { collisionSide_ = side; }

	void SetCollisionMask(uint32_t mask) { collisionMask_ = mask; }
};