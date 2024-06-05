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

class GameScene;

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

	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

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

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	Vector3 GetWorldPosition();

	float GetRadius() const { return radius_; }

	bool IsDead() const { return isDead_; }

private:
	// 関数ポインター配列
	static void (Enemy::*const pPhaseFunctionTable_[])();

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// プレイヤー
	player* player_ = nullptr;

	// 敵の状態
	BaseEnemyPhase* enemyPhase_;

	// 死亡フラグ
	bool isDead_ = false;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Input* input_ = nullptr;

	Vector3 velocity_;
	Vector3 position_;
	float radius_ = 1.0f;

	int32_t fireTimer_ = 0;

	// enum class Phase {
	//	Aproach,
	//	Leave,
	// };

	// Phase phase_ = Phase::Aproach;
};
