#include "followCamera.h"
#include "ImGuiManager.h"
#include "lockOn.h"

FollowCamera::FollowCamera() {}

FollowCamera::~FollowCamera() {}

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
	input_ = Input::GetInstance();
}

void FollowCamera::Update() {

	float rotateSpeed = 0.03f;

	if (lockOn_->isTargetExist()) {
		// ロックオン中はロックオン対象に向く
		Vector3 lockOnPos = lockOn_->GetTargetPos();
		Vector3 sub = lockOnPos - target_->translation_;
		float angle = std::atan2(sub.x, sub.z);
		viewProjection_.rotation_.y = angle;
		destinationAngleY_ = angle;
	}
	else {
		// ゲームパッドによる回転
		XINPUT_STATE joyState;
		if (input_->GetJoystickState(0, joyState)) {

			destinationAngleY_ += (float)joyState.Gamepad.sThumbRX * rotateSpeed * 0.0001f;

			// 右スティック押し込みで角度をターゲットの後ろにリセット
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
				destinationAngleY_ = target_->rotation_.y;
			}
		}

		// キーボードによる回転
		if (input_->PushKey(DIK_LEFT)) {
			viewProjection_.rotation_.y -= rotateSpeed;
		}
		if (input_->PushKey(DIK_RIGHT)) {
			viewProjection_.rotation_.y += rotateSpeed;
		}
	}

	// カメラの角度を目標角度に向けて補間
	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.15f);


	// playerの位置に補間して追従
	if (target_) {
		// ターゲットの位置に補間
		interTargetPos_ = Lerp(interTargetPos_, target_->translation_, t_);

		// ターゲットの位置にカメラを追従
		Vector3 offset = CalculateOffset();

		viewProjection_.translation_ = interTargetPos_ + offset;

		// カメラのビュー行列を更新
		viewProjection_.UpdateMatrix();
	}

}

void FollowCamera::Reset() { 
	if (target_) {
		interTargetPos_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}

	destinationAngleY_ = viewProjection_.rotation_.y;

	Vector3 offset = CalculateOffset();

	viewProjection_.translation_ = interTargetPos_ + offset;
}

Vector3 FollowCamera::CalculateOffset() const {
	Vector3 offset = {0.0f, 2.0f, -10.0f};

	// カメラの角度から回転行列を算出
	Matrix4x4 rotationMatrix = MakeRotateMatrixXYZ(viewProjection_.rotation_);

	offset = TransFormNormal(offset, rotationMatrix);

	return offset;
}

void FollowCamera::SetTarget(const WorldTransform* target) { 
	target_ = target;
	Reset();
}