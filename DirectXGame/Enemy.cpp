#include "Enemy.h"
#include "ImGuiManager.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(const std::vector<Model*> models) {
	BaseCharacter::Initialize(models);

	// ワールド変換データの初期化
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	// ワールド変換データの初期設定
	worldTransformBody_.translation_ = {0.0f, 0.8f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.85f, 0.15f, 0.0f};
	worldTransformR_arm_.translation_ = {0.85f, 0.15f, 0.0f};

	// モデル同士の親子関係を設定
	worldTransformBody_.SetParent(&worldTransform_);
	worldTransformL_arm_.SetParent(&worldTransformBody_);
	worldTransformR_arm_.SetParent(&worldTransformBody_);
}

void Enemy::Update() {

	UpdateFloatAnimation();

	Move();

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	ImGuiDraw();
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

	// worldTransformBody_.translation_.y = std::sin(floatingParam_) * amplitude;

	// 腕を揺らす
	worldTransformL_arm_.translation_.y = std::sin(floatingParam_) * amplitude;
	worldTransformR_arm_.translation_.y = std::sin(floatingParam_) * amplitude;
}
