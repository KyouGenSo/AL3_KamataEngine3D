#include "CollisionManager.h"


void CollisionManager::AddCollider(Collider* collider) { colliders_.push_back(collider); }

void CollisionManager::ClearList() { colliders_.clear(); }

void CollisionManager::CheckCollision(Collider* collider1, Collider* collider2) {
	Vector3 posA = collider1->GetWorldPosition();
	Vector3 posB = collider2->GetWorldPosition();
	float distance = Distance(posA, posB);

	if ((collider1->GetCollisionSide() & collider2->GetCollisionMask()) == 0 ||
		(collider2->GetCollisionSide() & collider1->GetCollisionMask()) == 0) {
		return;
	}

	if (distance <= collider1->GetRadius() + collider2->GetRadius()) {
		collider1->OnCollision();
		collider2->OnCollision();
	}
}

void CollisionManager::CheckAllCollision() {
	// 衝突判定
	std::list<Collider*>::iterator it1 = colliders_.begin();
	for (; it1 != colliders_.end(); ++it1) {

		std::list<Collider*>::iterator it2 = it1;

		for (++it2; it2 != colliders_.end(); ++it2) {

			CheckCollision(*it1, *it2);
		}
	}
}