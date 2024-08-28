#include "Enemy.h"
#include "ImGuiManager.h"

// global serial number
uint32_t Enemy::nextSerialNumber_ = 0;

Enemy::Enemy() {}

Enemy::~Enemy() { 
	serialNumber_ = nextSerialNumber_;
	nextSerialNumber_++;
}

Vector3 Enemy::GetCenter() const {
	Vector3 offset = offset_;
	Vector3 worldPos = TransForm(worldTransform_.matWorld_, offset);

	return worldPos;
}

void Enemy::Initialize(const std::vector<Model*> models) {
	BaseCharacter::Initialize(models);

	Collider::SetRadius(5.5f);

	// ワールド変換データの初期化
	worldTransformBody_.Initialize();

	// ワールド変換データの初期設定
	worldTransformBody_.translation_ = offset_;

	// モデル同士の親子関係を設定
	worldTransformBody_.SetParent(&worldTransform_);

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeId::kEnemy));
}

void Enemy::Update() {

	//UpdateFloatAnimation();

	if (!isHitStop_) {
		Move();
	}

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();

	if (isHitStop_) {
		hitStopTime_--;
		ShakeEffect();
		if (hitStopTime_ <= 0) {
			isHitStop_ = false;
			worldTransformBody_.translation_ = offset_;
		}
	}

	//ImGuiDraw();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// body描画
	models_[0]->Draw(worldTransformBody_, viewProjection);
}

void Enemy::ImGuiDraw() {
#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Body", &worldTransformBody_.translation_.x, 0.1f);
	ImGui::DragFloat("period", &period, 0.1f);
	ImGui::DragFloat("amplitude", &amplitude, 0.1f);
	ImGui::End();
#endif _DEBUG
}

void Enemy::Move() {
	float speed = 0.1f;
	// y軸回転
	worldTransform_.rotation_.y += 0.01f;

	// 向いてる方向に進む
	worldTransform_.translation_.x += std::sin(worldTransform_.rotation_.y) * speed;
	worldTransform_.translation_.y += std::sin(worldTransform_.rotation_.x) * speed;
	worldTransform_.translation_.z += std::cos(worldTransform_.rotation_.y) * speed;

	// 行列を更新
	worldTransform_.UpdateMatrix();

}

void Enemy::InitializeFloatAnimation() { floatingParam_ = 0.0f; }

void Enemy::UpdateFloatAnimation() {
	// 1フレームでの加算量
	float add = float(2.0f * M_PI / period);

	floatingParam_ += add;
	// 2πを超えたら0に戻す
	floatingParam_ = float(std::fmod(floatingParam_, 2.0f * M_PI));

	worldTransformBody_.translation_.y += std::sin(floatingParam_) * amplitude;

}

void Enemy::ShakeEffect() {
	// ランダムな値を生成
	float randomX = Rand(-0.15f, 0.15f);
	float randomY = Rand(-0.03f, 0.03f);
	float randomZ = Rand(-0.15f, 0.15f);


	worldTransformBody_.translation_.x += randomX;
	worldTransformBody_.translation_.y += randomY;
	worldTransformBody_.translation_.z += randomZ;
}

void Enemy::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手がプレイヤーである場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kPlayerWeapon)) {

	}

	// 衝突相手がhammerである場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kPlayerWeapon)) {
		isHitStop_ = true;
		hitStopTime_ = 7;
	}
}