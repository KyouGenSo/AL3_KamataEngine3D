#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>
#include <PrimitiveDrawer.h>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("./Resources/uvChecker.png");

	// スプライトの作成
	sprite_ = Sprite::Create(textureHandle_, {10, 10});
	spriteSize_ = {100, 100};
	sprite_->SetSize(spriteSize_);

	// 3Dモデルの作成
	model_ = Model::Create();

	// worldTransformとviewProjectionの初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// サウンドの読み込み
	soundHandle_ = audio_->LoadWave("./Resources/mokugyo.wav");

	// サウンドの再生
	voiceHandle_ = audio_->PlayWave(soundHandle_, true);

	// ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	// DeBugCameraの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	//// スプライトの座標を取得
	// Vector2 pos = sprite_->GetPosition();

	////座標を{2, 1}ずつ移動
	// pos.x += 2;
	// pos.y += 1;

	////画面外に出たら初期位置に戻す
	// if (pos.x > 1280) {
	//	pos.x = 10;
	// }
	// if (pos.y > 720) {
	//	pos.y = 10;
	// }

	////移動した座標をスプライトに反映
	// sprite_->SetPosition(pos);

	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voiceHandle_);
	}

	debugCamera_->Update();

#pragma region ImGui DeBug
	ImGui::Begin("DeBug1");

	ImGui::Text("KyouGenSo %d/%d/%d", 2001, 9, 15);
	ImGui::InputFloat3("InPutFloat3", inPutFloat3);
	ImGui::SliderFloat3("SliderFloat3", inPutFloat3, 0.0f, 0.1f);
	ImGui::ShowDemoWindow();

	ImGui::End();

#pragma endregion
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dモデルの描画
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	// ラインを描画
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライトの描画
	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
