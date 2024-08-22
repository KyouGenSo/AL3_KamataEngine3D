#include "collider.h"

void Collider::Initialize() {
	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3(radius_, radius_, radius_);
}

void Collider::UpdateWorldTransform() {
	// 中心座標を取得
	worldTransform_.translation_ = GetCenter();

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {
	model->Draw(worldTransform_, viewProjection);
}