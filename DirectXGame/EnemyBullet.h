#pragma once
#include "ImGuiManager.h"
#include "Input.h"
#include "Matrix4x4Function.h"
#include "Model.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "myFunction.h"
#include <algorithm>
#include <cassert>

class EnemyBullet {
public:

	const int32_t kLifeTime = 60 * 5;

	EnemyBullet();
	~EnemyBullet();

	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);

	void Update();

	void Draw(ViewProjection& viewProjection);
	
private:
	Model* model_;
	WorldTransform worldTransform_;
	Vector3 velocity_;
	uint32_t textureHandle_;

	
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};