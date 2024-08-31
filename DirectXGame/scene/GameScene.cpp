#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <PrimitiveDrawer.h>
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::ResetAllObjects() {
	player_->ReSet();
	enemy_->ReSet();
}

void GameScene::InitializeUI() {
	// UIの初期化
	playerHP_ = std::make_unique<Sprite>();
	playerHP_.reset(Sprite::Create(TextureManager::Load("white1x1.png"), playerHP_Pos_));
	playerHP_->SetSize(playerHP_Size_);

	enemyHP_ = std::make_unique<Sprite>();
	enemyHP_.reset(Sprite::Create(TextureManager::Load("red1x1.png"), enemyHP_Pos_));
	enemyHP_->SetSize(enemyHP_Size_);

	player_Text_ = std::make_unique<Sprite>();
	player_Text_.reset(Sprite::Create(TextureManager::Load("player_Text.png"), player_Text_Pos_));
	player_Text_->SetSize(textSize_);

	enemy_Text_ = std::make_unique<Sprite>();
	enemy_Text_.reset(Sprite::Create(TextureManager::Load("enemy_Text.png"), enemy_Text_Pos_));
	enemy_Text_->SetSize(textSize_);

	A_Icon_ = std::make_unique<Sprite>();
	A_Icon_.reset(Sprite::Create(TextureManager::Load("A_Icon.png"), A_Icon_Pos_));

	B_Icon_ = std::make_unique<Sprite>();
	B_Icon_.reset(Sprite::Create(TextureManager::Load("B_Icon.png"), B_Icon_Pos_));

	X_Icon_ = std::make_unique<Sprite>();
	X_Icon_.reset(Sprite::Create(TextureManager::Load("X_Icon.png"), X_Icon_Pos_));

	LT_Icon_ = std::make_unique<Sprite>();
	LT_Icon_.reset(Sprite::Create(TextureManager::Load("LT_Icon.png"), LT_Icon_Pos_));

	RT_Icon_ = std::make_unique<Sprite>();
	RT_Icon_.reset(Sprite::Create(TextureManager::Load("RT_Icon.png"), RT_Icon_Pos_));

	dash_Text_ = std::make_unique<Sprite>();
	dash_Text_.reset(Sprite::Create(TextureManager::Load("dash_Text.png"), dash_Text_Pos_));
	dash_Text_->SetSize(textSize_);

	jump_Text_ = std::make_unique<Sprite>();
	jump_Text_.reset(Sprite::Create(TextureManager::Load("jump_Text.png"), jump_Text_Pos_));
	jump_Text_->SetSize(textSize_);

	attack_Text_ = std::make_unique<Sprite>();
	attack_Text_.reset(Sprite::Create(TextureManager::Load("attack_Text.png"), attack_Text_Pos_));
	attack_Text_->SetSize(textSize_);

	aim_Text_ = std::make_unique<Sprite>();
	aim_Text_.reset(Sprite::Create(TextureManager::Load("aim_Text.png"), aim_Text_Pos_));
	aim_Text_->SetSize(textSize_);

	shoot_Text_ = std::make_unique<Sprite>();
	shoot_Text_.reset(Sprite::Create(TextureManager::Load("shoot_Text.png"), shoot_Text_Pos_));
	shoot_Text_->SetSize(textSize_);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// BGMの読み込み
	//bgmSH_ = audio_->LoadWave("inGameBGM.wav");
	//bgmVH_ = audio_->PlayWave(bgmSH_, true, 0.25f);

	// UIの初期化
	InitializeUI();

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
	playerWeaponModel_.reset(Model::CreateFromOBJ("weapon", true));             // プレイヤーの武器のモデル
	playerBulletModel_.reset(Model::CreateFromOBJ("playerBullet", true));       // プレイヤーの弾のモデル
	playerWeaponEffectModel_.reset(Model::CreateFromOBJ("efffect_ball", true)); // プレイヤーの武器のエフェクトのモデル
	playerModels_ = {playerHeadModel_.get(),   playerBodyModel_.get(),   playerL_armModel_.get(),       playerR_armModel_.get(),
	                 playerWeaponModel_.get(), playerBulletModel_.get(), playerWeaponEffectModel_.get()};

	// 敵のモデル
	enemyBodyModel_.reset(Model::CreateFromOBJ("boss", true));
	enemyBlockModel_.reset(Model::CreateFromOBJ("boss_block", true));
	enemyModels_ = {enemyBodyModel_.get(), enemyBlockModel_.get()};

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

	// 敵の初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels_);

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

	// SET------------------------------------------------------------
	// 追従カメラのViewProjectionをplayerに持たせる
	player_->SetCameraViewProjection(&followCamera_->GetViewProjection());

	// LockOnにプレイヤーをセット
	lockOn_->SetPlayer(player_.get());

	// 敵にプレイヤーをセット
	enemy_->SetPlayer(player_.get());

	// 敵に追従カメラをセット
	enemy_->SetFollowCamera(followCamera_.get());

	// プレイヤーに敵をセット
	player_->SetEnemy(enemy_.get());
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

	if (player_->GetHP() <= 0) {
		isGameOver_ = true;
	}else if (enemy_->GetHp() <= 0) {
		isGameClear_ = true;
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

#ifdef _DEBUG
	// imguiの表示
	ImGui::Begin("UI");
	// playerHP_Pos_
	ImGui::DragFloat2("playerHP_Pos", &playerHP_Pos_.x, 0.1f);
	playerHP_->SetPosition(playerHP_Pos_);
	// playerHP_Size_
	ImGui::DragFloat2("playerHP_Size", &playerHP_Size_.x, 0.1f);
	playerHP_->SetSize(playerHP_Size_);
	// enemyHP_Pos_
	ImGui::DragFloat2("enemyHP_Pos", &enemyHP_Pos_.x, 0.1f);
	enemyHP_->SetPosition(enemyHP_Pos_);
	// enemyHP_Size_
	ImGui::DragFloat2("enemyHP_Size", &enemyHP_Size_.x, 0.1f);
	enemyHP_->SetSize(enemyHP_Size_);
	// player_Text_Pos_
	ImGui::DragFloat2("player_Text_Pos", &player_Text_Pos_.x, 0.1f);
	player_Text_->SetPosition(player_Text_Pos_);
	// enemy_Text_Pos_
	ImGui::DragFloat2("enemy_Text_Pos", &enemy_Text_Pos_.x, 0.1f);
	enemy_Text_->SetPosition(enemy_Text_Pos_);

	ImGui::End();
#endif _DEBUG
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

	// UIの描画
	A_Icon_->Draw();
	B_Icon_->Draw();
	X_Icon_->Draw();
	LT_Icon_->Draw();
	RT_Icon_->Draw();
	dash_Text_->Draw();
	jump_Text_->Draw();
	attack_Text_->Draw();
	aim_Text_->Draw();
	shoot_Text_->Draw();
	player_Text_->Draw();
	enemy_Text_->Draw();

	// HPの描画
	DrawHP();

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

	std::list<EnemyBlock*> blocks_ = enemy_->GetBlocks();
	for (EnemyBlock* block : blocks_) {
		collisionManager_->AddCollider(block);
	}

	collisionManager_->CheckAllCollisions();
}

void GameScene::DrawHP() {
	float playerHP = player_->GetHP();
	float enemyHP = enemy_->GetHp();

	playerHP_->SetSize({playerHP * 5.0f, 30.0f});
	enemyHP_->SetSize({enemyHP * 5.0f, 40.0f});

	// プレイヤーのHPの描画
	playerHP_->Draw();
	// 敵のHPの描画
	enemyHP_->Draw();
}
