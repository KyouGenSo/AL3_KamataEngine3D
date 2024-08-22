#pragma once
#pragma once
#include "Matrix4x4Function.h"
#include "Model.h"
#include "Vector3Function.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include <cmath>
#include "memory"
#include "myFunction.h"
#include "collisionTypeIdDef.h"

// 親クラス
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	///</summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*> models) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

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
	/// ImGuiによるデバッグ表示
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// Getters
	/// </summary>
	Vector3 GetCenter() const override;
	uint32_t GetSerialNumber() const { return serialNumber_; }
	

	/// <summary>
	/// Setters
	/// </summary>


private: // メンバ変数

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	// シリアルナンバー
	uint32_t serialNumber_ = 0;

	static uint32_t nextSerialNumber_;

	//const ViewProjection* cameraViewProjection_;

	// ----------------------浮遊アニメーション用---------------------
	float floatingParam_ = 0.0f;
	// 周期
	float period = 60.0f; // 60フレームで1周期
	// 振幅
	float amplitude = 0.08f;
};