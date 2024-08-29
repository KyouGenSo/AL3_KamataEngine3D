#include "Enemy.h"
#include "ImGuiManager.h"
#include "Player.h"

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

	Collider::SetRadius(5.4f);

	// ワールド変換データの初期化
	worldTransformBody_.Initialize();

	// ワールド変換データの初期設定
	worldTransformBody_.translation_ = offset_;

	// モデル同士の親子関係を設定
	worldTransformBody_.SetParent(&worldTransform_);

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeId::kEnemy));

	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));
}

void Enemy::Update() {

	// UpdateFloatAnimation();

	// playerとの距離を計算
	toPlayerV_ = player_->GetCenter() - GetCenter();
	toPlayerDis_ = toPlayerV_.length();

	// 行動遷移
	if (behaviorRequest_) {
		// behavior_を変更する
		behavior_ = behaviorRequest_.value();
		// 各行動の初期化処理
		switch (behavior_) {
		case Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		case Behavior::kNear:
			BehaviorNearInitialize();
			break;
		case Behavior::kAway:
			BehaviorAwayInitialize();
			break;
		case Behavior::kFarAttack1:
			BehaviorFarAttack1Initialize();
			break;
		case Behavior::kFarAttack2:
			BehaviorFarAttack2Initialize();
			break;
		case Behavior::kFarAttack3:
			BehaviorFarAttack3Initialize();
			break;
		case Behavior::kNearAttack1:
			BehaviorNearAttack1Initialize();
			break;
		case Behavior::kNearAttack2:
			BehaviorNearAttack2Initialize();
			break;
		case Behavior::kNearAttack3:
			BehaviorNearAttack3Initialize();
			break;
		}
		// behaviorRequest_をリセット
		behaviorRequest_ = std::nullopt;
	}

	// 行動遷移の更新
	switch (behavior_) {

	case Behavior::kRoot:
		BehaviorRootUpdate();
		break;

	case Behavior::kNear:
		BehaviorNearUpdate();
		break;

	case Behavior::kAway:
		BehaviorAwayUpdate();
		break;
	case Behavior::kFarAttack1:
		BehaviorFarAttack1Update();
		break;

	case Behavior::kFarAttack2:
		BehaviorFarAttack2Update();
		break;
	case Behavior::kFarAttack3:
		BehaviorFarAttack3Update();
		break;

	case Behavior::kNearAttack1:
		BehaviorNearAttack1Update();
		break;

	case Behavior::kNearAttack2:
		BehaviorNearAttack2Update();
		break;

	case Behavior::kNearAttack3:
		BehaviorNearAttack3Update();
		break;
	}

	// if (!isHitStop_) {
	//	Move();
	// }

	BaseCharacter::Update();
	// 行列を更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();

	if (isHitStop_) {
		hitStopTime_--;
		ShakeEffect();
		if (hitStopTime_ <= 0) {
			isHitStop_ = false;
			worldTransformBody_.translation_ = offset_;
		}
	}

	if (behaviorCD_ > 0) {
		behaviorCD_--;
	}

	// ImGuiDraw();
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
	// toPlayerDis
	ImGui::Text("toPlayerDis: %f", toPlayerDis_);
	// rand
	ImGui::Text("rand: %f", rand_);
	// randIndex
	ImGui::Text("randIndex: %d", randIndex_);
	// behavior
	ImGui::Text("behavior: %d", static_cast<int>(behavior_));
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

// ----------------------行動遷移用---------------------
void Enemy::BehaviorRootInitialize() {
	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));

	behaviorCD_ = 60 * 3;
}
void Enemy::BehaviorRootUpdate() {

	if (behaviorCD_ <= 0) {
		// playerとの距離が近い場合
		if (toPlayerDis_ < 30.0f) {
			switch (randIndex_) {
			case 0:
				behaviorRequest_ = Behavior::kAway;
				break;
			case 1:
				behaviorRequest_ = Behavior::kNearAttack1;
				break;
			case 2:
				behaviorRequest_ = Behavior::kNearAttack2;
				break;
			case 3:
				behaviorRequest_ = Behavior::kNearAttack3;
				break;
			}
		}

		// playerとの距離が遠い場合
		else {
			switch (randIndex_) {
			case 0:
				behaviorRequest_ = Behavior::kNear;
				break;
			case 1:
				behaviorRequest_ = Behavior::kFarAttack1;
				break;
			case 2:
				behaviorRequest_ = Behavior::kFarAttack2;
				break;
			case 3:
				behaviorRequest_ = Behavior::kFarAttack3;
				break;
			}
		}
	}
}
// ----------------------------------------------------Near
void Enemy::BehaviorNearInitialize() {
	// 行動遷移の初期化処理
	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));
}
void Enemy::BehaviorNearUpdate() {
	float speed = 0.5f;

	if (toPlayerDis_ > 25.0f) {
		worldTransform_.translation_.x += toPlayerV_.normalize().x * speed;
		worldTransform_.translation_.z += toPlayerV_.normalize().z * speed;
	} else {
		behaviorRequest_ = Behavior::kRoot;
	}
}
// ----------------------------------------------------Away
void Enemy::BehaviorAwayInitialize() {
	// 行動遷移の初期化処理
	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));
}
void Enemy::BehaviorAwayUpdate() {
	float speed = 0.9f;

	// playerの逆方向に進む
	if (toPlayerDis_ < 50.0f) {
		worldTransform_.translation_.x -= toPlayerV_.normalize().x * speed;
		worldTransform_.translation_.z -= toPlayerV_.normalize().z * speed;
	} else {
		behaviorRequest_ = Behavior::kRoot;
	}

	worldTransform_.UpdateMatrix();
}
// ----------------------------------------------------Far1
void Enemy::BehaviorFarAttack1Initialize() {
	// 行動遷移の初期化処理
	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));
}
void Enemy::BehaviorFarAttack1Update() {
	// 行動遷移の更新処理
	behaviorRequest_ = Behavior::kRoot;
}
// ----------------------------------------------------Far2
void Enemy::BehaviorFarAttack2Initialize() {
	// 行動遷移の初期化処理
	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));
}
void Enemy::BehaviorFarAttack2Update() {
	// 行動遷移の更新処理
	behaviorRequest_ = Behavior::kRoot;
}
// ----------------------------------------------------Far3
void Enemy::BehaviorFarAttack3Initialize() {
	// 行動遷移の初期化処理
	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));
}
void Enemy::BehaviorFarAttack3Update() {
	// 行動遷移の更新処理
	behaviorRequest_ = Behavior::kRoot;
}
// ----------------------------------------------------Near1
void Enemy::BehaviorNearAttack1Initialize() {
	// 行動遷移の初期化処理
	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));
}
void Enemy::BehaviorNearAttack1Update() {
	// 行動遷移の更新処理
	behaviorRequest_ = Behavior::kRoot;
}
// ----------------------------------------------------Near2
void Enemy::BehaviorNearAttack2Initialize() {
	// 行動遷移の初期化処理
	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));
}
void Enemy::BehaviorNearAttack2Update() {
	// 行動遷移の更新処理
	behaviorRequest_ = Behavior::kRoot;
}
// ----------------------------------------------------Near3
void Enemy::BehaviorNearAttack3Initialize() {
	// 行動遷移の初期化処理
	rand_ = Rand(0.0f, 3.0f);
	randIndex_ = int(std::floor(rand_));
}
void Enemy::BehaviorNearAttack3Update() {
	// 行動遷移の更新処理
	behaviorRequest_ = Behavior::kRoot;
}
// ----------------------------------------------------