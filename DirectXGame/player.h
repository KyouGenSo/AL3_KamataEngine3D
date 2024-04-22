#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

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
	void Draw();

private: // メンバ変数

	// モデル
	Model* model_ = nullptr;
	// ワールド変換
	WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
};