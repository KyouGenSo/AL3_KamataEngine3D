#pragma once
#include "Input.h"
#include "Matrix4x4Function.h"
#include "Model.h"
#include "WorldTransform.h"
#include <algorithm>

class Enemy;

class BaseEnemyPhase {

public:
	virtual ~BaseEnemyPhase() = default;
	virtual void Update(Enemy* enemy) = 0;

};

class EnemyPhaseAproach : public BaseEnemyPhase {
public:
	void Update(Enemy* enemy) override;

};

class EnemyPhaseLeave : public BaseEnemyPhase {
public:
	void Update(Enemy* enemy) override;

};

class Enemy {

public:
	Enemy();
	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void ChangePhase(BaseEnemyPhase* phase);

	void Draw(const ViewProjection& viewProjection);

	void Aproach();
	void Leave();

	//------------------------------------------------------------------------------------------------
	void SetTranlation(const Vector3& velocity) { worldTransform_.translation_ += velocity; }

	float GetTranslationX() const { return worldTransform_.translation_.x; }
	float GetTranslationY() const { return worldTransform_.translation_.y; }
	float GetTranslationZ() const { return worldTransform_.translation_.z; }

private:
	// 関数ポインター配列
	static void (Enemy::*const pPhaseFunctionTable_[])();

	BaseEnemyPhase* enemyPhase_;

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
