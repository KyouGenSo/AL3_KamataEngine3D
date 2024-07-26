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

	// ゲームパッドによる移動
	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {

		Vector3 move = {(float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY};

		move = move.normalize() * speed;

		Matrix4x4 rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);

		move = TransFormNormal(move, rotationMatrix);

		worldTransform_.translation_ += move;
	}

	// キーボードによる移動
	if (input_->PushKey(DIK_W)) {
		Vector3 move = {0.0f, 0.0f, speed};
		Matrix4x4 rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		move = TransFormNormal(move, rotationMatrix);
		worldTransform_.translation_ += move;
	}

	if (input_->PushKey(DIK_S)) {
		Vector3 move = {0.0f, 0.0f, -speed};
		Matrix4x4 rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		move = TransFormNormal(move, rotationMatrix);
		worldTransform_.translation_ += move;
	}

	if (input_->PushKey(DIK_A)) {
		Vector3 move = {-speed, 0.0f, 0.0f};
		Matrix4x4 rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		move = TransFormNormal(move, rotationMatrix);
		worldTransform_.translation_ += move;
	}

	if (input_->PushKey(DIK_D)) {
		Vector3 move = {speed, 0.0f, 0.0f};
		Matrix4x4 rotationMatrix = MakeRotateMatrixXYZ(cameraViewProjection_->rotation_);
		move = TransFormNormal(move, rotationMatrix);
		worldTransform_.translation_ += move;
	}

	worldTransform_.UpdateMatrix();
}