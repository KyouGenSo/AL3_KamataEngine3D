#include "Player.h"
#include "ImGuiManager.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* modelHead, Model* modelBody, Model* modelL_arm, Model* modelR_arm) {
	assert(modelHead);
	assert(modelBody);
	assert(modelL_arm);
	assert(modelR_arm);

	modelHead_ = modelHead;
	modelBody_ = modelBody;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	worldTransformBase_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	// モデルの初期位置を設定
	worldTransformHead_.translation_ = {0.0f, 1.5f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.55f, 1.3f, 0.0f};
	worldTransformR_arm_.translation_ = {0.55f, 1.3f, 0.0f};

	// モデル同士の親子関係を設定
	worldTransformBody_.SetParent(&worldTransformBase_);
	worldTransformHead_.SetParent(&worldTransformBody_);
	worldTransformL_arm_.SetParent(&worldTransformBody_);
	worldTransformR_arm_.SetParent(&worldTransformBody_);

	input_ = Input::GetInstance();

	InitializeFloatAnimation();
}

void Player::Update() {

	Move();

	UpdateFloatAnimation();

	worldTransformBase_.TransferMatrix();

	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	ImGuiDraw();
}

void Player::ImGuiDraw() {
	ImGui::Begin("Player");
	ImGui::DragFloat3("Head", &worldTransformHead_.translation_.x, 0.1f);
	ImGui::DragFloat3("L_arm", &worldTransformL_arm_.translation_.x, 0.1f);
	ImGui::DragFloat3("R_arm", &worldTransformR_arm_.translation_.x, 0.1f);
	ImGui::DragFloat("period", &period, 0.1f);
	ImGui::DragFloat("amplitude", &amplitude, 0.1f);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	// ヘッドの描画
	modelHead_->Draw(worldTransformHead_, viewProjection);

	// 胴体の描画
	modelBody_->Draw(worldTransformBody_, viewProjection);

	// 左腕の描画
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);

	// 右腕の描画
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);

}

void Player::Move() {
	const float speed = 0.3f;
	Vector3 move_;
	Matrix4x4 rotationMatrix;

	// ゲームパッドによる移動
	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {
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

			worldTransformBase_.translation_ += move_;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		}
	} else { // キーボードによる移動

		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A)) {
			move_ = {-speed, 0.0f, speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransformBase_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
			move_ = {speed, 0.0f, speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransformBase_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A)) {
			move_ = {-speed, 0.0f, -speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransformBase_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D)) {
			move_ = {speed, 0.0f, -speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransformBase_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_W)) {
			move_ = {0.0f, 0.0f, speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransformBase_.translation_ += move_;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_S)) {
			move_ = {0.0f, 0.0f, -speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransformBase_.translation_ += move_;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_A)) {
			move_ = {-speed, 0.0f, 0.0f};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransformBase_.translation_ += move_;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		} else if (input_->PushKey(DIK_D)) {
			move_ = {speed, 0.0f, 0.0f};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransformBase_.translation_ += move_;

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
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, targetAngle_, t_);

	worldTransformBase_.UpdateMatrix();
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