#include "Enemy.h"
#include "player.h"
#include "GameScene.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	delete enemyPhase_;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = {0.0f, 0.0f, -0.5f};

	enemyPhase_ = new EnemyPhaseAproach();

	enemyPhase_->Init();
}

void Enemy::Update() {
	enemyPhase_->Update(this);

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Fire() {

	assert(player_);

	const float kBulletSpeed = -1.0f;

	Vector3 bulletVelocity = {0.0f, 0.0f, 0.0f};
	// bulletVelocity = TransFormNormal(bulletVelocity, worldTransform_.matWorld_);

	// playerの座標を取得
	Vector3 playerPos = player_->GetWorldPosition();
	// enemyの座標を取得
	Vector3 enemyPos = GetWorldPosition();
	// enemyからplayerへの差分ベクトルを計算
	Vector3 diff;
	diff.x = enemyPos.x - playerPos.x;
	diff.y = enemyPos.y - playerPos.y;
	diff.z = enemyPos.z - playerPos.z;
	// 差分ベクトルを正規化
	diff = Normalize(diff);
	// 差分ベクトルをbulletVelocityに代入
	bulletVelocity.x += diff.x * kBulletSpeed;
	bulletVelocity.y += diff.y * kBulletSpeed;
	bulletVelocity.z += diff.z * kBulletSpeed;

	EnemyBullet* newBullet_ = new EnemyBullet();
	newBullet_->Initialize(model_, worldTransform_.translation_, bulletVelocity);

	gameScene_->AddEnemyBullet(newBullet_);
}

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision() {
	isDead_ = true; 
}

void Enemy::ChangePhase(BaseEnemyPhase* phase) {
	delete enemyPhase_;
	enemyPhase_ = phase;
	enemyPhase_->Init();
}

void EnemyPhaseAproach::Init() { fireTimer_ = kFireInterval; }

void EnemyPhaseLeave::Init() { fireTimer_ = kFireInterval; }

void EnemyPhaseAproach::Update(Enemy* enemy) {

	fireTimer_--;

	if (fireTimer_ <= 0) {
		enemy->Fire();
		fireTimer_ = kFireInterval;
	}

	Vector3 velocity_ = {0.0f, 0.0f, -0.1f};
	enemy->SetTranlation(velocity_);

	Vector3 enemyPos = enemy->GetWorldPosition();

	if (enemyPos.z <= 80.0f) {
		enemy->ChangePhase(new EnemyPhaseLeave());
	}
}

void EnemyPhaseLeave::Update(Enemy* enemy) {

	fireTimer_--;

	if (fireTimer_ <= 0) {
		enemy->Fire();
		fireTimer_ = kFireInterval;
	}

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	enemy->SetTranlation(velocity_);

	// Vector3 enemyPos = enemy->GetWorldPosition();

	// if (enemyPos.x >= 10.0f) {
	//	enemy->ChangePhase(new EnemyPhaseAproach());
	// }
}

