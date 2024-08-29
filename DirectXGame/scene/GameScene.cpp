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

	// worldTransformとviewProjectionの初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// collisionMnagerの生成と初期化
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	// 3Dモデルの作成----------------------------------------------------
	// プレイヤーのモデル
	playerHeadModel_.reset(Model::CreateFromOBJ("float_Head", true));
	playerBodyModel_.reset(Model::CreateFromOBJ("float_Body", true));
	playerL_armModel_.reset(Model::CreateFromOBJ("float_L_arm", true));
	playerR_armModel_.reset(Model::CreateFromOBJ("float_R_arm", true));
	playerWeaponModel_.reset(Model::CreateFromOBJ("weapon", true));// プレイヤーの武器のモデル
	playerBulletModel_.reset(Model::CreateFromOBJ("playerBullet", true)); // プレイヤーの弾のモデル
	playerWeaponEffectModel_.reset(Model::CreateFromOBJ("efffect_ball", true)); // プレイヤーの武器のエフェクトのモデル
	playerModels_ = {
		playerHeadModel_.get(), 
		playerBodyModel_.get(), 
		playerL_armModel_.get(), 
		playerR_armModel_.get(), 
		playerWeaponModel_.get(), 
		playerBulletModel_.get(),
		playerWeaponEffectModel_.get()
	};

	// 敵のモデル
	enemyBodyModel_.reset(Model::CreateFromOBJ("boss", true));
	enemyHitEffectModel_.reset(Model::CreateSphere());
	enemyModels_ = {enemyBodyModel_.get(), enemyHitEffectModel_.get()};

	// Skydomeのモデル
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));

	// 地面のモデル
	groundModel_.reset(Model::CreateFromOBJ("ground", true));

	// 初期化------------------------------------------------------------
	// LockOnの初期化
	lockOnMarkTexture_ = TextureManager::Load("aim_sphere.png");
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize(lockOnMarkTexture_);

	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels_);
	player_->SetLockOn(lockOn_.get());

	// LockOnにプレイヤーをセット
	lockOn_->SetPlayer(player_.get());

	// 敵の初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels_);

	// プレイヤーに敵をセット
	player_->SetEnemy(enemy_.get());

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
	followCamera_->SetLockOn(lockOn_.get());

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

	// 敵の更新
	enemy_->Update();

	// LockOnの更新
	lockOn_->Update(enemy_, viewProjection_);

	// 追従カメラの更新
	followCamera_->Update();
	if (!isDebugCameraActive_) {
		SetFollowCamera(followCamera_->GetViewProjection());
	}

	// Skydomeの更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

	// デバッグ表示用にトランスフォームを更新
	collisionManager_->UpdateWorldTransform();

	// 衝突判定と応答
	CheckAllCollisions();

	// プレイヤーのデバッグ表示
	player_->ImGuiDraw();
	// 敵のデバッグ表示
	enemy_->ImGuiDraw();
	// 当たり判定のデバッグ表示
	collisionManager_->ImGuiDraw();

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

	// 敵の描画
	enemy_->Draw(viewProjection_);

	// Skydomeの描画
	skydome_->Draw(viewProjection_);

	// 地面の描画
	ground_->Draw(viewProjection_);

	// プレイヤーの描画
	player_->Draw(viewProjection_);

	// 当たり判定の表示用モデルの描画
	collisionManager_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	lockOn_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::SetFollowCamera(ViewProjection& viewProjection) {
	viewProjection_.matView = viewProjection.matView;
	viewProjection_.matProjection = viewProjection.matProjection;
	viewProjection_.TransferMatrix();
}

void GameScene::CheckAllCollisions() {

	collisionManager_->Reset();

	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(enemy_.get());
	collisionManager_->AddCollider(player_->GetHammer());
	std::list<PlayerBullet*> bullets = player_->GetBullets();
	for (PlayerBullet* bullet : bullets) {
		collisionManager_->AddCollider(bullet);
	}

	collisionManager_->CheckAllCollisions();
}
