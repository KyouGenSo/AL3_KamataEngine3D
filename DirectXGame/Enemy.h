#pragma once
#pragma once
#include "Matrix4x4Function.h"
#include "Model.h"
#include "Vector3Function.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include "collisionTypeIdDef.h"
#include "memory"
#include "myFunction.h"
#include <cmath>
#include <optional>

// 親クラス
#include "BaseCharacter.h"

class Player;

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
	/// シェークエフェクト
	/// </summary>
	void ShakeEffect();

	/// <summary>
	/// ImGuiによるデバッグ表示
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// Behaviors
	/// </summary>
	void BehaviorRootInitialize(); // 通常状態の初期化
	void BehaviorRootUpdate();     // 通常状態の更新

	void BehaviorNearInitialize(); // 接近状態の初期化
	void BehaviorNearUpdate();     // 接近状態の更新

	void BehaviorAwayInitialize(); // 離脱状態の初期化
	void BehaviorAwayUpdate();     // 離脱状態の更新

	void BehaviorFarAttack1Initialize(); // 遠距離攻撃1状態の初期化
	void BehaviorFarAttack1Update();     // 遠距離攻撃1状態の更新

	void BehaviorFarAttack2Initialize(); // 遠距離攻撃2状態の初期化
	void BehaviorFarAttack2Update();     // 遠距離攻撃2状態の更新

	void BehaviorFarAttack3Initialize(); // 遠距離攻撃3状態の初期化
	void BehaviorFarAttack3Update();     // 遠距離攻撃3状態の更新

	void BehaviorNearAttack1Initialize(); // 近距離攻撃1状態の初期化
	void BehaviorNearAttack1Update();     // 近距離攻撃1状態の更新

	void BehaviorNearAttack2Initialize(); // 近距離攻撃2状態の初期化
	void BehaviorNearAttack2Update();     // 近距離攻撃2状態の更新

	void BehaviorNearAttack3Initialize(); // 近距離攻撃3状態の初期化
	void BehaviorNearAttack3Update();     // 近距離攻撃3状態の更新

	/// <summary>
	/// Getters
	/// </summary>
	Vector3 GetCenter() const override;
	uint32_t GetSerialNumber() const { return serialNumber_; }

	/// <summary>
	/// Setters
	/// </summary>
	void SetPlayer(const Player* player) { player_ = player; }

private: // メンバ変数
	// player参照
	const Player* player_ = nullptr;

	WorldTransform worldTransformBody_;

	// シリアルナンバー
	uint32_t serialNumber_ = 0;

	static uint32_t nextSerialNumber_;

	Vector3 offset_ = {0.0f, 3.9f, 0.0f};

	// ヒットストップ用
	bool isHitStop_ = false;
	uint32_t hitStopTime_ = 7;

	// hp
	float hp_ = 100.0f;

	// ----------------------行動遷移用---------------------
	enum class Behavior {
		kRoot,
		kNear,
		kAway,
		kFarAttack1,
		kFarAttack2,
		kFarAttack3,
		kNearAttack1,
		kNearAttack2,
		kNearAttack3,
	};

	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	float rand_;    // 乱数
	int randIndex_; // 乱数カウント

	Vector3 toPlayerV_;
	float toPlayerDis_;

	uint32_t behaviorCD_ = 0;

	// ----------------------行動遷移用---------------------

	// ----------------------浮遊アニメーション用---------------------
	float floatingParam_ = 0.0f;
	// 周期
	float period = 60.0f; // 60フレームで1周期
	// 振幅
	float amplitude = 0.08f;
};