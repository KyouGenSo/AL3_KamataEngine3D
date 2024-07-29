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

// Game Object
#include "Player.h"
#include "Skydome.h"
#include "Ground.h"
#include "followCamera.h"

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

	void SetFollowCamera(ViewProjection& viewProjection);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	uint32_t textureHandle_ = 0;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	// -----Game Object----- //

	// Player
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Model> playerHeadModel_ = nullptr;
	std::unique_ptr<Model> playerBodyModel_ = nullptr;
	std::unique_ptr<Model> playerL_armModel_ = nullptr;
	std::unique_ptr<Model> playerR_armModel_ = nullptr;
	//std::vector<std::unique_ptr<Model>> playerModels_;
	//std::unique_ptr<std::vector<Model*>> playerModels_;
	
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
