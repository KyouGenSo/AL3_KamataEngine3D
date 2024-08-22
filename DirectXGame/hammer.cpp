#include "hammer.h"
#include "enemy.h"

void Hammer::Initialize(Model* model, Model* effectModel) {
	model_ = model;
	effectModel_ = effectModel;
	effectModel_->SetAlpha(0.5f);

	SetRadius(1.0f);

	Collider::Initialize();

	hammerWorldTransform_.Initialize();
	effectWorldTransform_.Initialize();

	hammerWorldTransform_.translation_ = Vector3(0.0f, 0.8f, 0.0f);
	effectWorldTransform_.scale_ = Vector3(0.0f, 0.0f, 0.0f);

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeId::kPlayerWeapon));
}

void Hammer::Update() {
	if (isHit_) {
		effectWorldTransform_.scale_.x += 0.8f;
		effectWorldTransform_.scale_.y += 0.8f;
		effectWorldTransform_.scale_.z += 0.8f;

		if (effectWorldTransform_.scale_.x >= 2.5f) {
			effectWorldTransform_.scale_ = Vector3(0.0f, 0.0f, 0.0f);
			isHit_ = false;
		}
	}


	hammerWorldTransform_.UpdateMatrix();
	effectWorldTransform_.UpdateMatrix();
}

void Hammer::Draw(const ViewProjection& viewProjection) {
	model_->Draw(hammerWorldTransform_, viewProjection);

	if (isHit_) {
		effectModel_->Draw(effectWorldTransform_, viewProjection);
	}
}

void Hammer::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵である場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kEnemy)) {
		// 衝突相手を敵クラスにダウンキャスト
		Enemy* enemy = static_cast<Enemy*>(other);
		uint32_t serialNum = enemy->GetSerialNumber();

		// すでに衝突している敵である場合は処理を終了
		if (collisionRecord_.CheckRecord(serialNum)) {
			return;
		}

		// 衝突した敵のシリアルナンバーを記録
		collisionRecord_.AddRecord(serialNum);

		// 敵の位置にeffectを表示
		effectWorldTransform_.translation_ = enemy->GetCenter();
		isHit_ = true;
	}

}

Vector3 Hammer::GetCenter() const {
	Vector3 offset = {0.0f, 5.0f, 0.0f};
	Vector3 worldPos = TransForm(hammerWorldTransform_.matWorld_, offset);

	return worldPos;
}