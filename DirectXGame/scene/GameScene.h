#pragma once

#include "Audio.h"
#include "ImGuiManager.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "AxisIndicator.h"
#include "memory"
#include "vector"
#include "collisionManager.h"

// Game Object
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "Ground.h"
#include "followCamera.h"
#include "lockOn.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	void SetFollowCamera(ViewProjection& viewProjection);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	// 当たり判定用
	std::unique_ptr<CollisionManager> collisionManager_;

	// -----Game Object----- //

	// Player
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Model> playerHeadModel_ = nullptr;
	std::unique_ptr<Model> playerBodyModel_ = nullptr;
	std::unique_ptr<Model> playerL_armModel_ = nullptr;
	std::unique_ptr<Model> playerR_armModel_ = nullptr;
	std::unique_ptr<Model> playerWeaponModel_ = nullptr; // weapon
	std::vector<Model*> playerModels_;

	// LockOn system
	std::unique_ptr<LockOn> lockOn_ = nullptr;
	uint32_t lockOnMarkTexture_ = 0;

	// enemy
	std::unique_ptr<Enemy> enemy_ = nullptr;
	std::unique_ptr<Model> enemyBodyModel_ = nullptr;
	std::unique_ptr<Model> enemyL_armModel_ = nullptr;
	std::unique_ptr<Model> enemyR_armModel_ = nullptr;
	std::unique_ptr<Model> enemyHitEffectModel_ = nullptr;
	std::vector<Model*> enemyModels_;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Model> skydomeModel_ = nullptr;

	// 地面
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Model> groundModel_ = nullptr;

	// -----Game Object----- //

	/// <summary>
	/// DeBug用
	/// </summary>
	bool isDebugCameraActive_ = false;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

};
