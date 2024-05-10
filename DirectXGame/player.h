#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix4x4Function.h"
#include <cassert>
#include "ImGuiManager.h"
#include "myFunction.h"
#include <algorithm>
#include "PlayerBullet.h"

// 自キャラクラス
class player {

public: // メンバ関数

	// コンストラクタ
	player();
	// デストラクタ
	~player();
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);
	// 毎フレーム処理
	void Update();
	// 描画
	void Draw(ViewProjection& viewProjection);
	// 旋回
	void Rotate();
	// 攻撃
	void Attack();

private: // メンバ変数

	// モデル
	Model* model_ = nullptr;
	// ワールド変換
	WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//キーボード入力
	Input* input_ = nullptr;

	// プレイヤーの弾
	playerBullet* bullet_ = nullptr;

};