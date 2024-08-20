#pragma once
#include "Input.h"
#include "Xinput.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Matrix4x4Function.h"
#include "Vector3Function.h"
#include "myFunction.h"
#include <math.h>

class FollowCamera {

public: // メンバ関数
	FollowCamera();
	~FollowCamera();

	void Initialize();
	void Update();

	void Reset();

	// offsetの計算関数
	Vector3 CalculateOffset() const;

	/// <summary>
	/// Setters
	/// </summary>
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// Getters
	/// </summary>
	ViewProjection& GetViewProjection() { return viewProjection_; }

private: // メンバ変数
	ViewProjection viewProjection_;

	const WorldTransform* target_ = nullptr;

	Input* input_ = nullptr;

	Vector3 interTargetPos_;

	float t_ = 0.18f;

	float destinationAngleY_ = 0.0f;

};
