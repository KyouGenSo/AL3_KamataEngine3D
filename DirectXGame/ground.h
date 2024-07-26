#pragma once
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "memory"
#include <cassert>

class Ground {
public:
	Ground();
	~Ground();

	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	std::unique_ptr<Model> model_ = nullptr;
	WorldTransform worldTransform_;
};