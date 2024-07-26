#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include "memory"
#include "Xinput.h"
#include "Input.h"

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
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

private: // メンバ変数

	Input* input_ = nullptr;


	/// <summary>
	/// プレイヤー用
	/// </summary>
	uint32_t textureHandle_ = 0;

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

};