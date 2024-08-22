#include "Player.h"
#include "ImGuiManager.h"


Player::Player() {}

Player::~Player() {
	models_.clear();
	models_.shrink_to_fit();
}

Vector3 Player::GetCenter() const {
	Vector3 offset = {0.0f, 1.5f, 0.0f};
	Vector3 worldPos = TransForm(worldTransform_.matWorld_, offset);

	return worldPos;
}

void Player::Initialize(const std::vector<Model*> models) {

	SetRadius(collisionRadius_);

	BaseCharacter::Initialize(models);

	// ワールド変換データの初期化
	// worldTransform_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	// モデルの初期位置を設定
	worldTransformHead_.translation_ = {0.0f, 1.5f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.55f, 1.3f, 0.0f};
	worldTransformR_arm_.translation_ = {0.55f, 1.3f, 0.0f};

	// モデル同士の親子関係を設定
	worldTransformBody_.SetParent(&worldTransform_);
	worldTransformHead_.SetParent(&worldTransformBody_);
	worldTransformL_arm_.SetParent(&worldTransformBody_);
	worldTransformR_arm_.SetParent(&worldTransformBody_);

	// 武器の初期化
	hammer_ = std::make_unique<Hammer>();
	hammer_->Initialize(models[4], Model::CreateSphere());
	hammer_->SetParent(worldTransformBody_);

	input_ = Input::GetInstance();

	InitializeFloatAnimation();

	// Colliderの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeId::kPlayer));
}

void Player::Update() {

	// 行動遷移
	if (behaviorRequest_) {
		// behavior_を変更する
		behavior_ = behaviorRequest_.value();
		// 各行動の初期化処理
		switch (behavior_) {
		case Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		case Behavior::kJump:
			BehaviorJumpInitialize();
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

	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;

	case Behavior::kDash:
		BehaviorDashUpdate();
		break;

	case Behavior::kJump:
		BehaviorJumpUpdate();
		break;
	}

	// 行列の更新
	BaseCharacter::Update();
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	// 武器の更新
	hammer_->Update();
}

void Player::Draw(const ViewProjection& viewProjection) {
	// ヘッドの描画
	models_[0]->Draw(worldTransformHead_, viewProjection);

	// 胴体の描画
	models_[1]->Draw(worldTransformBody_, viewProjection);

	// 左腕の描画
	models_[2]->Draw(worldTransformL_arm_, viewProjection);

	// 右腕の描画
	models_[3]->Draw(worldTransformR_arm_, viewProjection);

	// 武器の描画
	if (enableWeapon_) {
		hammer_->Draw(viewProjection);
	}
}

void Player::ImGuiDraw() {
#ifdef _DEBUG
	ImGui::Begin("Player");
	if (ImGui::BeginTabBar("Option")) {

		if (ImGui::BeginTabItem("Body")) {
			ImGui::DragFloat3("translation", &worldTransformBody_.translation_.x, 0.1f);
			ImGui::DragFloat3("rotation", &worldTransformBody_.rotation_.x, 0.1f);
			ImGui::DragFloat3("scale", &worldTransformBody_.scale_.x, 0.1f);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Head")) {
			ImGui::DragFloat3("translation", &worldTransformHead_.translation_.x, 0.1f);
			ImGui::DragFloat3("rotation", &worldTransformHead_.rotation_.x, 0.1f);
			ImGui::DragFloat3("scale", &worldTransformHead_.scale_.x, 0.1f);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("L_arm")) {
			ImGui::DragFloat3("translation", &worldTransformL_arm_.translation_.x, 0.1f);
			ImGui::DragFloat3("rotation", &worldTransformL_arm_.rotation_.x, 0.1f);
			ImGui::DragFloat3("scale", &worldTransformL_arm_.scale_.x, 0.1f);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("R_arm")) {
			ImGui::DragFloat3("translation", &worldTransformR_arm_.translation_.x, 0.1f);
			ImGui::DragFloat3("rotation", &worldTransformR_arm_.rotation_.x, 0.1f);
			ImGui::DragFloat3("scale", &worldTransformR_arm_.scale_.x, 0.1f);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("FloatAnim")) {
			ImGui::DragFloat("period", &period, 0.1f);
			ImGui::DragFloat("amplitude", &amplitude, 0.1f);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
#endif _DEBUG
}

void Player::Move() {
	const float speed = 0.3f;
	Matrix4x4 rotationMatrix;

	if (input_->GetJoystickState(0, joyState_)) { // ゲームパッドによる移動
		const float deadzone = 0.24f;
		bool isMoving = false;

		velocity_ = {(float)joyState_.Gamepad.sThumbLX, 0.0f, (float)joyState_.Gamepad.sThumbLY};

		if (Length(velocity_) > deadzone) {
			isMoving = true;
		}

		if (isMoving) {
			velocity_ = velocity_.normalize() * speed;

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);

			velocity_ = TransFormNormal(velocity_, rotationMatrix);

			worldTransform_.translation_ += velocity_;

			targetAngle_ = std::atan2(velocity_.x, velocity_.z);

			t_ = 0.0f;
		}
	}

	// キーボードによる移動
	if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A)) {
		velocity_ = {-speed, 0.0f, speed};

		rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		velocity_ = TransFormNormal(velocity_, rotationMatrix);

		worldTransform_.translation_ += velocity_.normalize() * speed;

		targetAngle_ = std::atan2(velocity_.x, velocity_.z);

		t_ = 0.0f;
	} else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
		velocity_ = {speed, 0.0f, speed};

		rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		velocity_ = TransFormNormal(velocity_, rotationMatrix);

		worldTransform_.translation_ += velocity_.normalize() * speed;

		targetAngle_ = std::atan2(velocity_.x, velocity_.z);

		t_ = 0.0f;
	} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A)) {
		velocity_ = {-speed, 0.0f, -speed};

		rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		velocity_ = TransFormNormal(velocity_, rotationMatrix);

		worldTransform_.translation_ += velocity_.normalize() * speed;

		targetAngle_ = std::atan2(velocity_.x, velocity_.z);

		t_ = 0.0f;
	} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D)) {
		velocity_ = {speed, 0.0f, -speed};

		rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		velocity_ = TransFormNormal(velocity_, rotationMatrix);

		worldTransform_.translation_ += velocity_.normalize() * speed;

		targetAngle_ = std::atan2(velocity_.x, velocity_.z);

		t_ = 0.0f;
	} else if (input_->PushKey(DIK_W)) {
		velocity_ = {0.0f, 0.0f, speed};

		rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		velocity_ = TransFormNormal(velocity_, rotationMatrix);

		worldTransform_.translation_ += velocity_;

		targetAngle_ = std::atan2(velocity_.x, velocity_.z);

		t_ = 0.0f;
	} else if (input_->PushKey(DIK_S)) {
		velocity_ = {0.0f, 0.0f, -speed};

		rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		velocity_ = TransFormNormal(velocity_, rotationMatrix);

		worldTransform_.translation_ += velocity_;

		targetAngle_ = std::atan2(velocity_.x, velocity_.z);

		t_ = 0.0f;
	} else if (input_->PushKey(DIK_A)) {
		velocity_ = {-speed, 0.0f, 0.0f};

		rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		velocity_ = TransFormNormal(velocity_, rotationMatrix);

		worldTransform_.translation_ += velocity_;

		targetAngle_ = std::atan2(velocity_.x, velocity_.z);

		t_ = 0.0f;
	} else if (input_->PushKey(DIK_D)) {
		velocity_ = {speed, 0.0f, 0.0f};

		rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		velocity_ = TransFormNormal(velocity_, rotationMatrix);

		worldTransform_.translation_ += velocity_;

		targetAngle_ = std::atan2(velocity_.x, velocity_.z);

		t_ = 0.0f;
	}

	if (t_ < 1.0f) {
		t_ += 0.1f;
	} else {
		t_ = 1.0f;
	}

	// ターゲットの角度に向かって回転
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngle_, t_);

	worldTransform_.UpdateMatrix();
}

void Player::InitializeFloatAnimation() { floatingParam_ = 0.0f; }

void Player::UpdateFloatAnimation() {
	// 1フレームでの加算量
	float add = float(2.0f * M_PI / period);

	floatingParam_ += add;
	// 2πを超えたら0に戻す
	floatingParam_ = float(std::fmod(floatingParam_, 2.0f * M_PI));

	worldTransformBody_.translation_.y = std::sin(floatingParam_) * amplitude;

	// 腕を揺らす
	worldTransformL_arm_.rotation_.x = std::sin(floatingParam_) * amplitude;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParam_) * amplitude;
}

void Player::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();

	// 衝突相手が敵である場合
	if (typeID == static_cast<uint32_t>(CollisionTypeId::kEnemy)) {
		behaviorRequest_ = Behavior::kJump;
	}
}

// ----------------------行動遷移用---------------------
// 通常状態
void Player::BehaviorRootInitialize() {
	// 浮遊アニメーション変数の初期化
	floatingParam_ = 0.0f;
	period = 130.0f;
	amplitude = 0.15f;

	// 腕の角度の初期化
	worldTransformL_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformR_arm_.rotation_ = {0.0f, 0.0f, 0.0f};

	attackRecovryTime_ = 15.0f;
}
void Player::BehaviorRootUpdate() {
	enableWeapon_ = false;

	attackRecovryTime_ -= 1.0f;

	if (input_->GetJoystickState(0, joyState_)) {
		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			if (attackRecovryTime_ <= 0.0f)
				behaviorRequest_ = Behavior::kAttack;
		}

		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			behaviorRequest_ = Behavior::kDash;
		}

		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			behaviorRequest_ = Behavior::kJump;
		}
	}

	// 移動
	Move();

	// 浮遊アニメーション
	UpdateFloatAnimation();
}

// 攻撃状態
void Player::BehaviorAttackInitialize() {
	workAttack_.isPreAttack_ = true;
	workAttack_.isPreAttack_ = true;
	workAttack_.isAttack_ = false;
	workAttack_.preAttackAngle_ = -3.1f;
	workAttack_.attackAngle_ = -1.2f;

	attackRecovryTime_ = 30.0f;

	hammer_->SetRotation({1.1f, 0.0f, 0.0f});
	hammer_->ClearCollisionRecord();
}
void Player::BehaviorAttackUpdate() {
	enableWeapon_ = true;
	// 攻撃処理
	if (workAttack_.isPreAttack_) { // 予備動作
		if (worldTransformL_arm_.rotation_.x > workAttack_.preAttackAngle_ || worldTransformR_arm_.rotation_.x > workAttack_.preAttackAngle_) {
			worldTransformL_arm_.rotation_.x -= 0.2f;
			worldTransformR_arm_.rotation_.x -= 0.2f;

			if (hammer_->GetRotation().x > 0.0f)
				hammer_->SetRotation({hammer_->GetRotation().x - 0.1f, 0.0f, 0.0f});
		} else {
			workAttack_.isAttack_ = true;
			workAttack_.isPreAttack_ = false;
		}
	}

	if (workAttack_.isAttack_) { // 攻撃動作

		if (worldTransformL_arm_.rotation_.x < workAttack_.attackAngle_ || worldTransformR_arm_.rotation_.x < workAttack_.attackAngle_) {
			worldTransformL_arm_.rotation_.x += 0.3f;
			worldTransformR_arm_.rotation_.x += 0.3f;

			if (hammer_->GetRotation().x < 1.6f)
				hammer_->SetRotation({hammer_->GetRotation().x + 0.2f, 0.0f, 0.0f});

		} else {
			attackRecovryTime_ -= 1.0f;
			if (attackRecovryTime_ <= 0.0f) {
				behaviorRequest_ = Behavior::kRoot;
			}
		}
	}
}

// ダッシュ状態
void Player::BehaviorDashInitialize() {
	workDash_.dashParam = 0;
	worldTransform_.rotation_.y = targetAngle_;
}
void Player::BehaviorDashUpdate() {
	// 今向いてる方向に移動する
	float speed = 1.8f;
	worldTransform_.translation_.x += std::sin(worldTransform_.rotation_.y) * speed;
	worldTransform_.translation_.z += std::cos(worldTransform_.rotation_.y) * speed;

	const uint32_t kDashTime = 10;

	if (++workDash_.dashParam >= kDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

// ジャンプ状態
void Player::BehaviorJumpInitialize() { 
	worldTransformBody_.translation_.y = 0.0f; 
	worldTransformL_arm_.rotation_.x = 0.0f;
	worldTransformR_arm_.rotation_.x = 0.0f;

	// ジャンプの初速
	const float kJumpSpeed = 0.8f;

	velocity_.y = kJumpSpeed;
}
void Player::BehaviorJumpUpdate() {
	worldTransform_.translation_ += velocity_;

	// 重力
	const float kGravity = 0.05f;
	Vector3 acceleration = {0.0f, -kGravity, 0.0f};

	velocity_ += acceleration;

	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0.0f;
		behaviorRequest_ = Behavior::kRoot;
	}
}

// ----------------------行動遷移用---------------------//