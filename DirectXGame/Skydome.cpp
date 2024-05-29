#include"Skydome.h"

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransform_.scale_ = { 500.0f, 500.0f, 500.0f };
	worldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };
}

void Skydome::Update() {
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}