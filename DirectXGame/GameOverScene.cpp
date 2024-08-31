#include "GameOverScene.h"

GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {}

void GameOverScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	// sprite
	backGround_ = std::make_unique<Sprite>();
	backGround_.reset(Sprite::Create(TextureManager::Load("black_BG.png"), backGround_Pos_));
	backGround_->SetSize({1280.0f, 720.0f});

	titleText_ = std::make_unique<Sprite>();
	titleText_.reset(Sprite::Create(TextureManager::Load("gameOver_Text.png"), title_Pos_));

	pressButtonText_ = std::make_unique<Sprite>();
	pressButtonText_.reset(Sprite::Create(TextureManager::Load("title_text.png"), pressButton_Pos_));
	pressButtonText_->SetSize({250.0f, 50.0f});
}

void GameOverScene::Update() {

	// imgui
#ifdef _DEBUG
	ImGui::Begin("GameOverScene");
	// title_Pos_
	ImGui::DragFloat2("title_Pos", &title_Pos_.x, 0.1f);
	titleText_->SetPosition(title_Pos_);
	// pressButton_Pos_
	ImGui::DragFloat2("pressButton_Pos", &pressButton_Pos_.x, 0.1f);
	pressButtonText_->SetPosition(pressButton_Pos_);
	ImGui::End();
#endif _DEBUG
}

void GameOverScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// 背景スプライト描画
	backGround_->Draw();

	// タイトルスプライト描画
	titleText_->Draw();

	// プレスボタンテキストスプライト描画
	pressButtonText_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}