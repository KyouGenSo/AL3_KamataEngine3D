#pragma once

#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "Model.h"
#include "Sprite.h"
#include "TextureManager.h"

class GameClearScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameClearScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameClearScene();

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;

	// sprite
	std::unique_ptr<Sprite> backGround_ = nullptr;
	std::unique_ptr<Sprite> titleText_ = nullptr;
	std::unique_ptr<Sprite> pressButtonText_ = nullptr;

	Vector2 backGround_Pos_ = {0.0f, 0.0f};
	Vector2 title_Pos_ = {396.0f, 170.0f};
	Vector2 pressButton_Pos_ = {530.8f, 463.2f};
};
