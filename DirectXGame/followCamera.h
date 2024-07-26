#pragma once
#include "Input.h"
#include "Xinput.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Matrix4x4Function.h"
#include "Vector3Function.h"

class FollowCamera {

public: // メンバ関数
	FollowCamera();
	~FollowCamera();

	void Initialize();
	void Update();

	/// <summary>
	/// Setters
	/// </summary>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// Getters
	/// </summary>
	ViewProjection& GetViewProjection() { return viewProjection_; }

private: // メンバ変数
	ViewProjection viewProjection_;

	const WorldTransform* target_ = nullptr;

	Input* input_ = nullptr;

	// 回転速度
	const float rotateSpeed_ = 0.035f;
};
