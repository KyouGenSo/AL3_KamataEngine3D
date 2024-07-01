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
#include "sstream"
#include <vector>
#include "PrimitiveDrawer.h"

#include "player.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "CollisionManager.h"

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

	void DrawCatmullRom(const std::vector<Vector3>& controlPoints);

	Vector3 CatmullRomPosition(const std::vector<Vector3>& controlPoints, float t);

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	//void CheckAllCollision();

	/// <summary>
	/// 敵の生成
	/// </summary>
	void CreateEnemy(Vector3 Position);

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();

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
	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// テクスチャハンドル
	uint32_t playerTextureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	// モデルハンドル
	Model* model_ = nullptr;
	Model* skydomeModel_ = nullptr;

	// ビュー射影行列
	ViewProjection viewProjection_;

	bool isWaiting_ = false;
	int waitTimer_ = 0;

	// 衝突マネージャー
	CollisionManager* collisionManager_;

	// 曲線描画用頂点データ
	std::vector<Vector3> controlPoints_;


	/// <summary>
	/// 衝突判定関数
	/// </summary>
	/// <param name="collider1">判定対象1</param>
	/// <param name="collider2">判定対象2</param>
	//void CheckCollision(Collider* collider1, Collider* collider2);
};
