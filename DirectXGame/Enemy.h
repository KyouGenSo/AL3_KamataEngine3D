#pragma once
#include "EnemyBullet.h"
#include "Input.h"
#include "Matrix4x4Function.h"
#include "Model.h"
#include "WorldTransform.h"
#include <algorithm>
#include <list>
#include <Vector3.h>
#include <Vector3Function.h>

class Enemy;

class player;

static const int kFireInterval = 60;

class BaseEnemyPhase {

public:
	virtual ~BaseEnemyPhase() = default;
	virtual void Init() = 0;
	virtual void Update(Enemy* enemy) = 0;
};

class EnemyPhaseAproach : public BaseEnemyPhase {
public:
	void Init() override;
	void Update(Enemy* enemy) override;

private:
	uint32_t fireTimer_;
};

class EnemyPhaseLeave : public BaseEnemyPhase {
public:
	void Init() override;
	void Update(Enemy* enemy) override;

private:
	uint32_t fireTimer_;
};

class Enemy {

public:
	Enemy();
	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void ChangePhase(BaseEnemyPhase* phase);

	void Draw(ViewProjection& viewProjection);

	void Fire();

	void OnCollision();

	void Aproach();
	void Leave();

	//------------------------------------------------------------------------------------------------
	void SetTranlation(const Vector3& velocity) { worldTransform_.translation_ += velocity; }

	void SetPlayer(player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	float GetBulletRadius() const { return bullets_.front()->GetRadius(); }

	float GetRadius() const { return radius_; }

private:
	// 関数ポインター配列
	static void (Enemy::*const pPhaseFunctionTable_[])();

	player* player_ = nullptr;

	BaseEnemyPhase* enemyPhase_;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Input* input_ = nullptr;

	Vector3 velocity_;
	Vector3 position_;
	float radius_ = 1.0f;

	std::list<EnemyBullet*> bullets_;

	int32_t fireTimer_ = 0;

	// enum class Phase {
	//	Aproach,
	//	Leave,
	// };

	// Phase phase_ = Phase::Aproach;
};
