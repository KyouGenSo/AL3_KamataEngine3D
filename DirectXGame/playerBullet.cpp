#include"playerBullet.h"
#include "enemy.h"

PlayerBullet::PlayerBullet() {}

PlayerBullet::~PlayerBullet() {}

Vector3 PlayerBullet::GetCenter() const {
	Vector3 offset = {0.0f, 0.0f, 0.0f};
	Vector3 worldPos = TransForm(worldTransform_.matWorld_, offset);

	return worldPos;
}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	Collider::Initialize();

	model_ = model;

	worldTransform_.Initialize();

	//worldTransform_.scale_ = Vector3(0.5f, 0.5f, 3.0f);

	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);

	// 解法1
	Matrix4x4 thetaYRotationMatrix = MakeRotateMatrixY(std::atan2(velocity.y, velocity.z));
	Vector3 velocityZ = TransForm(thetaYRotationMatrix, velocity);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	worldTransform_.translation_ = position;

	worldTransform_.UpdateMatrix();

	velocity_ = velocity;
}

void PlayerBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	float speed = 1.2f;

	Vector3 toEnemy = enemy_->GetCenter() - GetCenter();

	velocity_ = Slerp(velocity_.normalize(), toEnemy.normalize(), t_) * speed;

	worldTransform_.translation_ += velocity_;

	Matrix4x4 yRotMat = MakeRotateMatrixY(atan2f(velocity_.x, velocity_.z));
	Vector3 velocityZ = TransForm(yRotMat, velocity_);
	worldTransform_.rotation_.x = atan2f(-velocityZ.y, velocityZ.z);
	worldTransform_.rotation_.y = atan2f(velocityZ.x, velocityZ.z);

	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void PlayerBullet::OnCollision([[maybe_unused]] Collider* other) { 

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

		isDead_ = true;
	}
	
}