#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete skydomeModel_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
	delete skydome_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	#ifdef _DEBUG
	// 軸方向表示の初期化
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	#endif

	// テクスチャの読み込み
	playerTextureHandle_ = TextureManager::Load("./Resources/uvChecker.png");
	enemyTextureHandle_ = TextureManager::Load("./Resources/enemy.png");

	// モデルの生成
	model_ = Model::Create();
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// ビュー射影行列の初期化
	viewProjection_.Initialize();

	// レールカメラの初期化
	railCamera_ = new RailCamera();
	Vector3 railCameraPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 railCameraRot = Vector3(0.0f, 0.1f, 0.0f);
	railCamera_->Initialize(railCameraPos, railCameraRot);

	// プレイヤーの生成
	player_ = new player();
	// プレイヤーの初期化
	Vector3 playerPos = Vector3(0.0f, 0.0f, 35.0f);
	player_->Initialize(model_, playerTextureHandle_, playerPos);
	// レールカメラにプレイヤーを設定
	player_->SetParent(&railCamera_->GetWorldTransform());

	// 敵の生成
	enemy_ = new Enemy();
	// 敵の初期化
	enemy_->Initialize(model_, enemyTextureHandle_);
	// Playerを借りてくる
	enemy_->SetPlayer(player_);

	// スカイドームの生成
	skydome_ = new Skydome();
	// スカイドームの初期化
	skydome_->Initialize(skydomeModel_);
}

void GameScene::Update() {

	#ifdef _DEBUG
	// デバッグカメラのアクティブ切り替え
	if (input_->TriggerKey(DIK_F1)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// ビュー射影行列の更新
		viewProjection_.matView = debugCamera_->GetViewMatrix();
		viewProjection_.matProjection = debugCamera_->GetProjectionMatrix();
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matView = railCamera_->GetViewMatrix();
		viewProjection_.matProjection = railCamera_->GetProjectionMatrix();

		ImGui::Begin("viewproj");
		ImGui::DragFloat3("translation", &viewProjection_.translation_.x, -100.0f, 100.0f);
		ImGui::DragFloat3("rotation", &viewProjection_.rotation_.x, -3.14f, 3.14f);
		ImGui::End();
		viewProjection_.TransferMatrix();
	}

	// レールカメラの更新
	railCamera_->Update();

	// プレイヤーの更新
	player_->Update();

	// 敵の更新
	enemy_->Update();

	// 天球の更新
	skydome_->Update();

	// 衝突判定
	CheckAllCollision();

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

	// スカイドームの描画
	skydome_->Draw(viewProjection_);

	// プレイヤーの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	enemy_->Draw(viewProjection_);

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

void GameScene::CheckAllCollision() {
	Vector3 posA, posB;

	const std::list<EnemyBullet*>& bullets = enemy_->GetBullets();
	const std::list<playerBullet*>& playerBullets = player_->GetBullets();

	#pragma region プレイヤーと敵弾の衝突判定
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : bullets) {
		posB = bullet->GetWorldPosition();
		if (Distance(posA, posB) <= player_->GetRadius() + enemy_->GetBulletRadius()) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 敵とプレイヤー弾の衝突判定
	posA = enemy_->GetWorldPosition();

	for (playerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		if (Distance(posA, posB) <= enemy_->GetRadius() + player_->GetBulletRadius()) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region プレイヤー弾と敵弾の衝突判定
	for (playerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : bullets) {
			posB = enemyBullet->GetWorldPosition();
			if (Distance(posA, posB) <= playerBullet->GetRadius() + enemyBullet->GetRadius()) {
				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}
	#pragma endregion
}
