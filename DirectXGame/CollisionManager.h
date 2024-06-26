#pragma once
#include "Collider.h"
#include <list>
#include <myFunction.h>

class CollisionManager {
private:
	std::list<Collider*> colliders_;

public:

	void AddCollider(Collider* collider);

	void ClearList();

	void CheckCollision(Collider* collider1, Collider* collider2);

	void CheckAllCollision();
};
