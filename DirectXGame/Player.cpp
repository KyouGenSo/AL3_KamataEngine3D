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

		move = move.Normalize() * speed;

		worldTransform_.translation_ += move;
	}

	// キーボードによる移動
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.z += speed;
	}

	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.z -= speed;
	}

	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= speed;
	}

	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += speed;
	}

	worldTransform_.UpdateMatrix();
}