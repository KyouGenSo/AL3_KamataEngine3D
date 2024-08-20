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
#include <optional>

// 親クラス
#include "BaseCharacter.h"

class Player : public BaseCharacter {

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
	/// Behaviors
	/// </summary>
	void BehaviorRootInitialize();
	void BehaviorRootUpdate();

	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();

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
	const WorldTransform& GetWorldTransformHead() const { return worldTransformHead_; }
	const WorldTransform& GetWorldTransformBody() const { return worldTransformBody_; }
	const WorldTransform& GetWorldTransformL_arm() const { return worldTransformL_arm_; }
	const WorldTransform& GetWorldTransformR_arm() const { return worldTransformR_arm_; }
	// ----------------------浮遊アニメーション用---------------------
	float GetFloatingParam() const { return floatingParam_; }
	float GetPeriod() const { return period; }
	float GetAmplitude() const { return amplitude; }

	/// <summary>
	/// Setters
	/// </summary>
	void SetCameraViewProjection(const ViewProjection* cameraViewProjection) { cameraViewProjection_ = cameraViewProjection; }


private: // メンバ変数

	Input* input_ = nullptr;

	/// <summary>
	/// プレイヤー用
	/// </summary>

	// ワールド変換データ
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;

	const ViewProjection* cameraViewProjection_;

	float targetAngle_ = 0.0f;
	float t_ = 0.0f;

	// ----------------------行動遷移用---------------------
	enum class Behavior {
		kRoot,
		kAttack,
	};

	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//----------------------攻撃アニメーション用---------------------
	bool enableWeapon_ = false;
	bool isPreAttack_ = true;
	bool isAttack_ = false;
	float preAttackAngle_ = -3.1f;
	float attackAngle_ = -1.2f;
	float attackRecovryTime_ = 15.0f;

	// ----------------------浮遊アニメーション用---------------------
	float floatingParam_ = 0.0f;
	// 周期
	float period = 130.0f; // 60フレームで1周期
	// 振幅
	float amplitude = 0.15f;
};