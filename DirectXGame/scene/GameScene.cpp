#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <PrimitiveDrawer.h>
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() { 
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

#ifdef _DEBUG
	// DebugCameraの初期化
	debugCamera_.reset(new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight));

	// 軸の初期化
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif _DEBUG

	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("./Resources/player/player.png");

	// 3Dモデルの作成
	playerHeadModel_.reset(Model::CreateFromOBJ("float_Head", true));
	playerBodyModel_.reset(Model::CreateFromOBJ("float_Body", true));
	playerL_armModel_.reset(Model::CreateFromOBJ("float_L_arm", true));
	playerR_armModel_.reset(Model::CreateFromOBJ("float_R_arm", true));

	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	groundModel_.reset(Model::CreateFromOBJ("ground", true));


	// worldTransformとviewProjectionの初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerHeadModel_.get(), playerBodyModel_.get(), playerL_armModel_.get(), playerR_armModel_.get());

	// Skydomeの初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// 地面の初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	// 追従カメラの初期化
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());

	// 追従カメラのViewProjectionをplayerに持たせる
	player_->SetCameraViewProjection(&followCamera_->GetViewProjection());
}

void GameScene::Update() {

#ifdef _DEBUG
	// デバッグカメラのアクティブ切り替え
	if (input_->TriggerKey(DIK_F1)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	// デバッグカメラの更新
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewMatrix();
		viewProjection_.matProjection = debugCamera_->GetProjectionMatrix();
		viewProjection_.TransferMatrix();
	}

	// プレイヤーの更新
	player_->Update();

	// 追従カメラの更新
	followCamera_->Update();
	if (!isDebugCameraActive_) {
		SetFollowCamera(followCamera_->GetViewProjection());
	}

	// Skydomeの更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();
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

	// プレイヤーの描画
	player_->Draw(viewProjection_);

	// Skydomeの描画
	skydome_->Draw(viewProjection_);

	// 地面の描画
	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::SetFollowCamera(ViewProjection& viewProjection) { 
	viewProjection_.matView = viewProjection.matView;
	viewProjection_.matProjection = viewProjection.matProjection;
	viewProjection_.TransferMatrix();
}
