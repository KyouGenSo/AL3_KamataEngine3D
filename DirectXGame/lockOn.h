#pragma once
#include "Enemy.h"
#include "Matrix4x4Function.h"
#include "Sprite.h"
#include "Xinput.h"
#include "Input.h"
#include "Vector3Function.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cmath>
#include "WinApp.h"

const float kDeg2Rad = float(M_PI / 180.0f);

class LockOn {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t Tex);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(const std::unique_ptr<Enemy>& target, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// LOCK ON対象の検索
	/// </summary>
	void SearchTarget(const std::unique_ptr<Enemy>& enemy, const ViewProjection& viewProjection);

	bool IsOutDistance(const std::unique_ptr<Enemy>& enemy, const ViewProjection& viewProjection);
	bool IsOutDistance(const ViewProjection& viewProjection);

private:
	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	std::unique_ptr<Sprite> lockOnMark_ = nullptr;

	// ロックオン対象
	const Enemy* target_ = nullptr;

	bool isLockOn_ = false;

	// 最小距離
	float minDis_ = 0.0f;
	// 最大距離
	float maxDis_ = 50.0f;
	// 角度範囲
	float angleRange_ = 30.0f * kDeg2Rad;
};
