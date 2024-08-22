#pragma once
#include "Vector3.h"
#include "collider.h"
#include "list"
#include "Model.h"

class CollisionManager {
private:
	std::list<Collider*> colliders_;

	std::unique_ptr<Model> collBallModel_;

	bool collBallVisibility_ = false;

public:
	void Initialize();

	void UpdateWorldTransform();

	void Draw(const ViewProjection& viewProjection);

	void ImGuiDraw();

	void Reset();

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	void CheckAllCollisions();

	void AddCollider(Collider* collider) {
		colliders_.push_back(collider);
	}

	void SetCollBallVisibility(bool visibility) { collBallVisibility_ = visibility; }
};