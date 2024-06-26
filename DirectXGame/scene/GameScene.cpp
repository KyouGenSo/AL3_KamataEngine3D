#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "fstream"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete skydomeModel_;
	delete player_;
	delete debugCamera_;
	delete skydome_;
	delete railCamera_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
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

	// レティクルのテクスチャ
	/*uint32_t reticleTextureHandle = */ TextureManager::Load("./Resources/reticle.png");

	// プレイヤーの生成
	player_ = new player();
	// プレイヤーの初期化
	Vector3 playerPos = Vector3(0.0f, 0.0f, 30.0f);
	player_->Initialize(model_, playerTextureHandle_, playerPos);
	// レールカメラにプレイヤーを設定
	player_->SetParent(&railCamera_->GetWorldTransform());

	// 敵の生成スクリプトの読み込み
	LoadEnemyPopData();

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
	player_->Update(viewProjection_);

	// 敵発生コマンドの更新
	UpdateEnemyPopCommands();

	// 敵の更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	// 死亡した敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// 敵弾の更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	// 死亡した敵弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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
	player_->Draw3D(viewProjection_);

	// 敵の描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	// 敵弾の描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	// レールカメラの描画
	railCamera_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CreateEnemy(Vector3 position) {
	Enemy* enemy = new Enemy();
	enemy->Initialize(model_, enemyTextureHandle_, position);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	enemies_.push_back(enemy);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }

void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (isWaiting_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			isWaiting_ = false;
		}
		return;
	}

	std::string command;

	while (getline(enemyPopCommands, command)) {
		std::istringstream line_stream(command);

		std::string word;
		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			Vector3 pos;

			getline(line_stream, word, ',');
			pos.x = (float)std::stof(word.c_str());

			getline(line_stream, word, ',');
			pos.y = (float)std::stof(word.c_str());

			getline(line_stream, word, ',');
			pos.z = (float)std::stof(word.c_str());

			CreateEnemy(pos);
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWaiting_ = true;
			waitTimer_ = waitTime;

			break;
		}
	}
}

void GameScene::CheckCollision(Collider* collider1, Collider* collider2) {
	Vector3 posA = collider1->GetWorldPosition();
	Vector3 posB = collider2->GetWorldPosition();
	float distance = Distance(posA, posB);

	if ((collider1->GetCollisionSide() & collider2->GetCollisionMask()) == 0 || 
		(collider2->GetCollisionSide() & collider1->GetCollisionMask()) == 0) {
		return;
	}

	if (distance <= collider1->GetRadius() + collider2->GetRadius()) {
		collider1->OnCollision();
		collider2->OnCollision();
	}
}

void GameScene::CheckAllCollision() {
	std::list<Collider*> colliders;

	// コライダーをリストに追加
	colliders.push_back(player_);

	for (Enemy* enemy : enemies_) {
		colliders.push_back(enemy);
	}

	for (playerBullet* playerBullet : player_->GetBullets()) {
		colliders.push_back(playerBullet);
	}

	for (EnemyBullet* enemyBullet : enemyBullets_) {
		colliders.push_back(enemyBullet);
	}

	// 衝突判定
	std::list<Collider*>::iterator it1 = colliders.begin();
	for (; it1 != colliders.end(); ++it1) {

		std::list<Collider*>::iterator it2 = it1;

		for (++it2; it2 != colliders.end(); ++it2) {

			CheckCollision(*it1, *it2);
		}
	}
}