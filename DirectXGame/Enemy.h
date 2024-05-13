#pragma once
#include "Matrix4x4Function.h"
#include "Model.h"
#include "WorldTransform.h"
#include <algorithm>

class Enemy {

public:
	Enemy();
	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	Vector3 velocity_;
	Vector3 position_;
};
