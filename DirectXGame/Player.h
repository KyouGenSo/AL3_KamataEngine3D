#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include "memory"
#include "Xinput.h"
#include "Input.h"
#include "Matrix4x4Function.h"
#include "Vector3Function.h"
#include "myFunction.h"

class Player {

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	///</summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* modelHead, Model* modelBody, Model* modelL_arm, Model* modelR_arm);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// ImGuiによるデバッグ表示
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 浮遊アニメーションの初期化
	/// </summary>
	void InitializeFloatAnimation();

	/// <summary>
	/// 浮遊アニメーションの更新処理
	/// </summary>
	void UpdateFloatAnimation();

	/// <summary>
	/// Getters
	/// </summary>
	WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	/// <summary>
	/// Setters
	/// </summary>
	void SetCameraViewProjection(const ViewProjection* cameraViewProjection) { cameraViewProjection_ = cameraViewProjection; }

private: // メンバ変数

	Input* input_ = nullptr;


	/// <summary>
	/// プレイヤー用
	/// </summary>

	Model* modelHead_ = nullptr;
	Model* modelBody_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;

	WorldTransform worldTransformBase_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	const ViewProjection* cameraViewProjection_;

	float targetAngle_ = 0.0f;
	float t_ = 0.0f;

	// ----------------------浮遊アニメーション用---------------------
	float floatingParam_ = 0.0f;
	// 　周期
	float period = 130.0f; // 60フレームで1周期
	// 振幅
	float amplitude = 0.15f;
};