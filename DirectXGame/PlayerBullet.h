#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix4x4Function.h"
#include <cassert>
#include "ImGuiManager.h"
#include "myFunction.h"
#include <algorithm>
#include "TextureManager.h"

class playerBullet {
public:
	playerBullet();
	~playerBullet();

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void OnCollision();

	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

	float GetRadius() const { return radius_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	Vector3 velocity_;

	float radius_ = 1.0f;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

};

