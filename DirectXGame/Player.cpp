#include "Player.h"
#include "ImGuiManager.h"

Player::Player() {}

Player::~Player() {
	models_.clear();
	models_.shrink_to_fit();
}

void Player::Initialize(const std::vector<Model*> models) {

	BaseCharacter::Initialize(models);

	// ワールド変換データの初期化
	// worldTransform_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformWeapon_.Initialize();

	// モデルの初期位置を設定
	worldTransformHead_.translation_ = {0.0f, 1.5f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.55f, 1.3f, 0.0f};
	worldTransformR_arm_.translation_ = {0.55f, 1.3f, 0.0f};
	worldTransformWeapon_.translation_ = {0.0f, 0.8f, 0.0f};

	// モデル同士の親子関係を設定
	worldTransformBody_.SetParent(&worldTransform_);
	worldTransformHead_.SetParent(&worldTransformBody_);
	worldTransformL_arm_.SetParent(&worldTransformBody_);
	worldTransformR_arm_.SetParent(&worldTransformBody_);
	worldTransformWeapon_.SetParent(&worldTransformBody_);

	input_ = Input::GetInstance();

	InitializeFloatAnimation();
}

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

void Player::BehaviorAttackInitialize() {
	isPreAttack_ = true;
	isAttack_ = false;
	preAttackAngle_ = -3.1f;
	attackAngle_ = -1.2f;
	attackRecovryTime_ = 15.0f;

	worldTransformWeapon_.rotation_.x = 1.1f;
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
	}

	// 行列の更新
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();

	// ImGuiによるデバッグ表示
	ImGuiDraw();
}

void Player::BehaviorRootUpdate() {
	enableWeapon_ = false;
	XINPUT_STATE joyState;
	
	attackRecovryTime_ -= 1.0f;

	if (input_->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			if (attackRecovryTime_ <= 0.0f)
			behaviorRequest_ = Behavior::kAttack;
		}
	}

	// 移動
	Move();

	// 浮遊アニメーション
	UpdateFloatAnimation();
}

void Player::BehaviorAttackUpdate() {
	enableWeapon_ = true;
	// 攻撃処理
	if (isPreAttack_) { // 予備動作
		if (worldTransformL_arm_.rotation_.x > preAttackAngle_ || worldTransformR_arm_.rotation_.x > preAttackAngle_) {
			worldTransformL_arm_.rotation_.x -= 0.2f;
			worldTransformR_arm_.rotation_.x -= 0.2f;

			if (worldTransformWeapon_.rotation_.x > 0.0f)
			worldTransformWeapon_.rotation_.x -= 0.1f;
		} else {
			isAttack_ = true;
			isPreAttack_ = false;
		}
	}

	if (isAttack_) { // 攻撃動作

		if (worldTransformL_arm_.rotation_.x < attackAngle_ || worldTransformR_arm_.rotation_.x < attackAngle_) {
			worldTransformL_arm_.rotation_.x += 0.3f;
			worldTransformR_arm_.rotation_.x += 0.3f;

			if (worldTransformWeapon_.rotation_.x < 1.6f)
			worldTransformWeapon_.rotation_.x += 0.2f;
			
		} else {
			attackRecovryTime_ -= 1.0f;
			if (attackRecovryTime_ <= 0.0f) {
				behaviorRequest_ = Behavior::kRoot;
			}
		}
	}

}

void Player::ImGuiDraw() {
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::Text("isPreAttack_: %s", isPreAttack_ ? "true" : "false");
	ImGui::Text("isAttack_: %s", isAttack_ ? "true" : "false");
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

		if (ImGui::BeginTabItem("Weapon")) {
			ImGui::DragFloat3("translation", &worldTransformWeapon_.translation_.x, 0.1f);
			ImGui::DragFloat3("rotation", &worldTransformWeapon_.rotation_.x, 0.1f);
			ImGui::DragFloat3("scale", &worldTransformWeapon_.scale_.x, 0.1f);
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
		models_[4]->Draw(worldTransformWeapon_, viewProjection);
	}
}

void Player::Move() {
	const float speed = 0.3f;
	Vector3 move_;
	Matrix4x4 rotationMatrix;

	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) { // ゲームパッドによる移動
		const float deadzone = 0.24f;
		bool isMoving = false;

		move_ = {(float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY};

		if (Length(move_) > deadzone) {
			isMoving = true;
		}

		if (isMoving) {
			move_ = move_.normalize() * speed;

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);

			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		}
	} else { // キーボードによる移動

		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A)) {
			move_ = {-speed, 0.0f, speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
			move_ = {speed, 0.0f, speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A)) {
			move_ = {-speed, 0.0f, -speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D)) {
			move_ = {speed, 0.0f, -speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_W)) {
			move_ = {0.0f, 0.0f, speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_S)) {
			move_ = {0.0f, 0.0f, -speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_A)) {
			move_ = {-speed, 0.0f, 0.0f};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_D)) {
			move_ = {speed, 0.0f, 0.0f};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		}
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