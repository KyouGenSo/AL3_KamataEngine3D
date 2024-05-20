#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	delete enemyPhase_;

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	position_ = {0.0f, 5.0f, 30.0f};
	worldTransform_.translation_ = position_;

	velocity_ = {0.0f, 0.0f, -0.5f};

	enemyPhase_ = new EnemyPhaseAproach();

	enemyPhase_->Init();
}

void Enemy::Update() {

	// switch (phase_) {
	// case Phase::Aproach:
	//	Aproach();
	//	break;
	// case Phase::Leave:
	//	Leave();
	//	break;
	// }

	//(this->*pPhaseFunctionTable_[static_cast<size_t>(phase_)])();

	enemyPhase_->Update(this);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

// void Enemy::Aproach() {
//	velocity_ = {0.0f, 0.0f, -0.5f};
//	worldTransform_.translation_ += velocity_;
//
//	if (worldTransform_.translation_.z <= 0.0f) {
//		phase_ = Phase::Leave;
//	}
// }
//
// void Enemy::Leave() {
//	velocity_ = {0.5f, 0.5f, 0.0f};
//	worldTransform_.translation_ += velocity_;
//
//	if (worldTransform_.translation_.x >= 10.0f) {
//		phase_ = Phase::Aproach;
//	}
// }

// void (Enemy::*const Enemy::pPhaseFunctionTable_[])() = {
//     &Enemy::Aproach, // 要素番号0
//     &Enemy::Leave,   // 要素番号1
// };

void Enemy::Fire() {
	const float kBulletSpeed = -1.0f;
	Vector3 bulletVelocity = {0.0f, 0.0f, kBulletSpeed};

	bulletVelocity = TransFormNormal(bulletVelocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet_ = new EnemyBullet();
	newBullet_->Initialize(model_, worldTransform_.translation_, bulletVelocity);

	bullets_.push_back(newBullet_);
}

void Enemy::ChangePhase(BaseEnemyPhase* phase) {
	delete enemyPhase_;
	enemyPhase_ = phase;
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

	if (enemy->GetTranslationZ() <= -5.0f) {
		enemy->ChangePhase(new EnemyPhaseLeave());
	}
}

void EnemyPhaseLeave::Update(Enemy* enemy) {
	Vector3 velocity_ = {0.5f, 0.5f, 0.0f};
	enemy->SetTranlation(velocity_);

	if (enemy->GetTranslationX() >= 10.0f) {
		enemy->ChangePhase(new EnemyPhaseAproach());
	}
}
