#include "followCamera.h"

FollowCamera::FollowCamera() {}

FollowCamera::~FollowCamera() {}

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
	input_ = Input::GetInstance();
}

void FollowCamera::Update() {

	float rotateSpeed = 0.003f;

	// ゲームパッドによる回転
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX * rotateSpeed * 0.001f;
	}

	// キーボードによる回転
	if (input_->PushKey(DIK_LEFT)) {
		viewProjection_.rotation_.y -= rotateSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		viewProjection_.rotation_.y += rotateSpeed;
	}

	// ターゲットが存在する場合
	if (target_) {
		// ターゲットの位置にカメラを追従
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		// カメラの角度から回転行列を算出
		Matrix4x4 rotationMatrix = MakeRotateMatrixXYZ(viewProjection_.rotation_);

		offset = TransFormNormal(offset, rotationMatrix);

		viewProjection_.translation_ = target_->translation_ + offset;
	}

	// カメラのビュー行列を更新
	viewProjection_.UpdateMatrix();
}