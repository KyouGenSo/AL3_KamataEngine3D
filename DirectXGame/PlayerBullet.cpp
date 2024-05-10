#include"PlayerBullet.h"

playerBullet::playerBullet() {}

playerBullet::~playerBullet() {}

void playerBullet::Initialize(Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("./Resources/black.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void playerBullet::Update() {
	worldTransform_.UpdateMatrix();
}

void playerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}