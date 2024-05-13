#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	position_ = {0.0f, 5.0f, 30.0f};
	worldTransform_.translation_ = position_;

	velocity_ = {0.0f, 0.0f, -0.5f};
}

void Enemy::Update() {
	switch (phase_) {
	case Phase::Aproach:
		Aproach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void Enemy::Aproach() {
	velocity_ = {0.0f, 0.0f, -0.5f};
	worldTransform_.translation_ += velocity_;

	if (worldTransform_.translation_.z <= 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	velocity_ = {0.5f, 0.5f, 0.0f};
	worldTransform_.translation_ += velocity_;
}