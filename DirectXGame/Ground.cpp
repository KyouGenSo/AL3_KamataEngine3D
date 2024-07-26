#include "ground.h"

Ground::Ground() {}

Ground::~Ground() {}

void Ground::Initialize(Model* model) {
	assert(model);
	model_.reset(model);

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };
}

void Ground::Update() {
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}