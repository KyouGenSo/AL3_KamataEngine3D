#include "lockOn.h"

void LockOn::Initialize(uint32_t Tex) {
	input_ = Input::GetInstance();

	lockOnMark_ = std::make_unique<Sprite>();
	lockOnMark_.reset(Sprite::Create(Tex, {0.0f, 0.0f}));
	lockOnMark_->SetSize({100.0f, 100.0f});
}

void LockOn::Update(const std::unique_ptr<Enemy>& enemy, const ViewProjection& viewProjection) {
	if (target_) {
		if (!IsOutDistance(viewProjection)) {
			target_ = nullptr;
		}
	}

	if (input_->GetJoystickState(0, joyState_)) {
		// ロックオンボタンが押されたら
		if (joyState_.Gamepad.bLeftTrigger > 0 && player_->GetBehavior() != 1) {
			// ロックオン対象の検索
			SearchTarget(enemy, viewProjection);
		} else {
			target_ = nullptr;
		}
	}

	if (target_) { // ロックオンマークの座標計算
		// ワールド座標
		Vector3 worldPos = enemy->GetCenter();
		// view projection変換
		Vector3 screenPos = TransForm(viewProjection.matView, worldPos);
		screenPos = TransForm(viewProjection.matProjection, screenPos);
		// viewport変換
		Matrix4x4 matViewport = MakeViewportMatrix(0.f, 0.f, WinApp::kWindowWidth, WinApp::kWindowHeight, 0.0f, 1.0f);
		screenPos = TransForm(matViewport, screenPos);
		// スクリーン2D座標
		Vector3 offset = {100.f / 2.0f, 100.f / 2.0f, 0.f};
		screenPos -= offset;
		Vector2 screenPos2D = {screenPos.x, screenPos.y};
		// ロックオンマークの座標設定
		lockOnMark_->SetPosition(screenPos2D);
	}
}

void LockOn::Draw() {
	if (target_) {
		lockOnMark_->Draw();
	}
}

void LockOn::SearchTarget(const std::unique_ptr<Enemy>& enemy, const ViewProjection& viewProjection) {

	// 角度条件チェック
	if (IsOutDistance(enemy, viewProjection)) {
		target_ = enemy.get();
	}
}

bool LockOn::IsOutDistance(const std::unique_ptr<Enemy>& enemy, const ViewProjection& viewProjection) {
	Vector3 worldPos = enemy->GetCenter();
	Vector3 viewPos = TransForm(viewProjection.matView, worldPos);

	if (minDis_ <= viewPos.z && viewPos.z <= maxDis_) {

		// 角度条件チェック
		float arcTan = std::atan2(std::sqrt(viewPos.x * viewPos.x + viewPos.y * viewPos.y), viewPos.z);
		if (std::abs(arcTan) <= angleRange_) {
			return true;
		}
	}

	return false;
}

bool LockOn::IsOutDistance(const ViewProjection& viewProjection) {
	Vector3 worldPos = target_->GetCenter();
	Vector3 viewPos = TransForm(viewProjection.matView, worldPos);

	if (minDis_ <= viewPos.z && viewPos.z <= maxDis_) {

		// 角度条件チェック
		float arcTan = std::atan2(std::sqrt(viewPos.x * viewPos.x + viewPos.y * viewPos.y), viewPos.z);
		if (std::abs(arcTan) <= angleRange_) {
			return true;
		}
	}

	return false;
}

Vector3 LockOn::GetTargetPos() const {
	if (isTargetExist()) {
		return target_->GetCenter();
	}

	return Vector3();
}