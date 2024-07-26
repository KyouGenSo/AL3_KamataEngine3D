#include "Player.h"

Player::Player() {}

Player::~Player() { delete model_; }

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();
}

void Player::Update() {

	Move();

	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

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
		}else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D)) {
			move_ = {speed, 0.0f, speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		}else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A)) {
			move_ = {-speed, 0.0f, -speed};

			rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
			move_ = TransFormNormal(move_, rotationMatrix);

			worldTransform_.translation_ += move_.normalize() * speed;

			targetAngle_ = std::atan2(move_.x, move_.z);

			t_ = 0.0f;
		}else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D)) {
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