#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "RailCamera.h"
#include "Skydome.h"
#include "myFunction.h"

#include "player.h"
#include "Enemy.h"
#include "EnemyBullet.h"

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
	void CheckAllCollision();

	/// <summary>
	/// 敵の生成
	/// </summary>
	void CreateEnemy(Vector3 Position);

	void AddEnemyBullet(EnemyBullet* enemyBullet);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// スカイドーム
	Skydome* skydome_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// プレイヤー
	player* player_ = nullptr;

	// 敵
	std::list<Enemy*> enemies_;
	std::list<EnemyBullet*> enemyBullets_;

	// テクスチャハンドル
	uint32_t playerTextureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	// モデルハンドル
	Model* model_ = nullptr;
	Model* skydomeModel_ = nullptr;

	// ビュー射影行列
	ViewProjection viewProjection_;
};
