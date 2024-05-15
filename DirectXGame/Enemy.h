#pragma once
#include "Matrix4x4Function.h"
#include "Model.h"
#include "WorldTransform.h"
#include <algorithm>
#include "Input.h"

class Enemy {

public:
	Enemy();
	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void Aproach();
	void Leave();

private:

	// 関数ポインター配列
	static void(Enemy::* const pPhaseFunctionTable_[])();

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Input* input_ = nullptr;

	Vector3 velocity_;
	Vector3 position_;

	enum class Phase {
		Aproach,
		Leave,
	};

	Phase phase_ = Phase::Aproach;
};
