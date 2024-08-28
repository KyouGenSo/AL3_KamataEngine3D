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
	Vector3 offset = {0.0f, 0.8f, 0.0f};
	Vector3 worldPos = TransForm(worldTransform_.matWorld_, offset);

	return worldPos;
}

void Enemy::Initialize(const std::vector<Model*> models) {
	BaseCharacter::Initialize(models);

	// ワールド変換データの初期化
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	// ワールド変換データの初期設定
	worldTransform_.matWorld_.m[3][0] = 10.0f;
	worldTransform_.matWorld_.m[3][1] = 0.8f;
	worldTransformBody_.translation_ = {0.0f, 0.8f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.85f, 0.15f, 0.0f};
	worldTransformR_arm_.translation_ = {0.85f, 0.15f, 0.0f};

	// モデル同士の親子関係を設定
	worldTransformBody_.SetParent(&worldTransform_);
	worldTransformL_arm_.SetParent(&worldTransformBody_);
	worldTransformR_arm_.SetParent(&worldTransformBody_);

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeId::kEnemy));
}

void Enemy::Update() {

	UpdateFloatAnimation();

	if (!isHitStop_) {
		Move();
	}

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	if (isHitStop_) {
		hitStopTime_--;
		ShakeEffect();
		if (hitStopTime_ <= 0) {
			isHitStop_ = false;
			worldTransformBody_.translation_ = {0.0f, 0.8f, 0.0f};
		}
	}

	//ImGuiDraw();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// body描画
	models_[0]->Draw(worldTransformBody_, viewProjection);
	// L_arm描画
	models_[1]->Draw(worldTransformL_arm_, viewProjection);
	// R_arm描画
	models_[2]->Draw(worldTransformR_arm_, viewProjection);
}

void Enemy::ImGuiDraw() {
#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Body", &worldTransformBody_.translation_.x, 0.1f);
	ImGui::DragFloat3("L_arm", &worldTransformL_arm_.translation_.x, 0.1f);
	ImGui::DragFloat3("R_arm", &worldTransformR_arm_.translation_.x, 0.1f);
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

	//worldTransformBody_.translation_.y += std::sin(floatingParam_) * amplitude;

	// 腕を揺らす
	worldTransformL_arm_.translation_.y = std::sin(floatingParam_) * amplitude;
	worldTransformR_arm_.translation_.y = std::sin(floatingParam_) * amplitude;
}

void Enemy::ShakeEffect() {
	// ランダムな値を生成
	float randomX = Rand(-0.1f, 0.1f);
	float randomY = Rand(-0.03f, 0.03f);
	float randomZ = Rand(-0.1f, 0.1f);


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
		hitStopTime_ = 8;
	}
}