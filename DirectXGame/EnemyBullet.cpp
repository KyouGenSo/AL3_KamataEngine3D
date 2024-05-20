#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(){}

EnemyBullet::~EnemyBullet(){}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) { 
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("./Resources/red.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_,viewProjection,textureHandle_);
}