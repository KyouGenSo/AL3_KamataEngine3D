#include"Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	position_ = { 0.0f, 5.0f, 20.0f };
	worldTransform_.translation_ = position_;

	velocity_ = { 0.0f, 0.0f, -0.5f };
}

void Enemy::Update() {

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}