#include "player.h"
#include "Enemy.h"

player::player() {}

player::~player() {
	// プレイヤーの弾の解放
	for (playerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
	delete model_;
	delete model3DReticle_;
	delete sprite2DReticleLockOn_;
}

// --------------------------------------------Getters-------------------------------------------- //
Vector3 player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 player::GetWorldPosition3DReticle() {
	Vector3 worldPos;

	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 player::GetWorldRotation() {
	Vector3 worldRot;
	worldRot.x = worldTransform_.rotation_.x;
	worldRot.y = worldTransform_.rotation_.y;
	worldRot.z = worldTransform_.rotation_.z;

	return worldRot;
}

// --------------------------------------------Setters-------------------------------------------- //
void player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

// --------------------------------------------Public functions-------------------------------------------- //
void player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {

	assert(model);
	model_ = model;
	model3DReticle_ = model;
	textureHandle_ = textureHandle;

	worldTransform3DReticle_.Initialize();

	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのスプライトハンドル
	uint32_t reticleTexture = TextureManager::Load("reticle.png");
	sprite2DReticle_ = Sprite::Create(reticleTexture, {500, 100}, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f));

	reticleTexture = TextureManager::Load("reticle_lockOn.png");
	sprite2DReticleLockOn_ = Sprite::Create(reticleTexture, {500, 100}, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f));

	input_ = Input::GetInstance();

	worldTransform_.Initialize();

	worldTransform_.translation_ = pos;

	worldTransform_.UpdateMatrix();
}

void player::Update(ViewProjection& viewProjection, std::list<Enemy*> enemies) {

	bullets_.remove_if([](playerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 旋回
	Rotate();

	// 移動
	Vector3 move = {0.0f, 0.0f, 0.0f};
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// ワールド変換の更新
	worldTransform_.translation_ += move;

	// 移動制限
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	// 攻撃
	for (Enemy* enemy : enemies) {
		if (enemy->IsDead()) {
			enemyLockOnCount_--;
			continue;
		}

		if (enemy->GetIsLocked()) {
			enemyLockOnCount_++;
			break;
		}
	}

	if (enemyLockOnCount_ > 1) {
		MultiAttack(enemies);
	} else {
		SingleAttack(enemies);
	}

	// プレイヤーの弾の更新
	for (playerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// アフィン変換行列の作成
	worldTransform_.UpdateMatrix();

	// 3Dレティクルの更新
	Update3DReticle(viewProjection, enemies);

	// レティクルのマルチロックオン
	ReticleMultiLockOn(viewProjection, enemies);

	// マルチロックオンの更新
	//UpdateMultiLockOn(viewProjection, enemies);


	// ImGui
	ImGui::Begin("Player Pos");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, -1.0f, 1.0f);
	ImGui::Text("x: %f, y: %f, z: %f", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	ImGui::Text("enemyLockOnCount_ : %d", enemyLockOnCount_);
	ImGui::End();
}

void player::Draw3D(ViewProjection& viewProjection) {
	// model3DReticle_->Draw(worldTransform3DReticle_, viewProjection);

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// プレイヤーの弾の描画
	for (playerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void player::DrawUI() {

	sprite2DReticle_->Draw();

}

void player::Rotate() {
	const float kRotateSpeed = 0.02f;

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotateSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotateSpeed;
	}
}

void player::SingleAttack(std::list<Enemy*> enemies) {
	if (input_->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 bulletVelocity = {0.0f, 0.0f, 0.0f};
		float minDis = 1000000.0f;

		if (isLockOn_) {
			for (Enemy* enemy : enemies) {
				if (enemy->IsDead()) {
					continue;
				}

				Vector3 E2PDiff = Subtract(enemy->GetWorldPosition(), GetWorldPosition());
				// float E2PDis = float(Length(E2PDiff));

				Vector2 enemy2DPos = {enemy->GetWorldPosition().x, enemy->GetWorldPosition().y};
				Vector2 reticle2DPos = {GetWorldPosition3DReticle().x, GetWorldPosition3DReticle().y};

				float E2RDis = float(Distance(enemy2DPos, reticle2DPos));
				if (E2RDis < minDis) {
					minDis = E2RDis;
					bulletVelocity = Multiply(Normalize(E2PDiff), kBulletSpeed);
				}
			}
		} else {
			Vector3 diff = Subtract(GetWorldPosition3DReticle(), GetWorldPosition());
			diff = Normalize(diff);
			bulletVelocity = Multiply(diff, kBulletSpeed);
		}

		Vector3 playerPos = GetWorldPosition();

		playerBullet* newBullet_ = new playerBullet();
		newBullet_->Initialize(model_, playerPos, bulletVelocity);

		bullets_.push_back(newBullet_);
	}
}

void player::MultiAttack(std::list<Enemy*> enemies) {
	if (input_->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 bulletVelocity = {0.0f, 0.0f, 0.0f};

		for (Enemy* enemy : enemies) {
			if (enemy->IsDead()) {
				continue;
			}

			if (enemy->GetIsLocked()) {
				Vector3 E2PDiff = Subtract(enemy->GetWorldPosition(), GetWorldPosition());
				bulletVelocity = Multiply(Normalize(E2PDiff), kBulletSpeed);

				Vector3 playerPos = GetWorldPosition();

				playerBullet* newBullet_ = new playerBullet();
				newBullet_->Initialize(model_, playerPos, bulletVelocity);

				bullets_.push_back(newBullet_);
			}
		}
	}
}

void player::Update3DReticle(ViewProjection& viewProjection, std::list<Enemy*> enemies) {

	// 3Dレティクルのワールド座標変換
	const float kReticleDistance = 20.0f;
	Vector3 offset = {0.0f, 0.0f, 1.0f};

	offset = TransFormNormal(offset, worldTransform_.matWorld_);
	offset = Multiply(Normalize(offset), kReticleDistance);
	worldTransform3DReticle_.translation_ = Add(worldTransform_.translation_, offset);
	worldTransform3DReticle_.UpdateMatrix();

	Matrix4x4 matViewPort = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProjectionViewPort = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewPort);

	// 3Dレティクルのワールド座標から2Dスクリーン座標への変換
	Vector3 reticlePos = GetWorldPosition3DReticle();
	reticlePos = TransForm(matViewProjectionViewPort, reticlePos);

	sprite2DReticle_->SetPosition(Vector2(reticlePos.x, reticlePos.y));

	// sprite2DReticleLockOn_->SetPosition(Vector2(reticlePos.x, reticlePos.y));

	// レティクルのシングルロックオン
	// ReticleSingleLockOn(viewProjection, enemies);
}

void player::ReticleSingleLockOn(ViewProjection& viewProjection, std::list<Enemy*> enemies) {
	float lockOnRange = 30.0f;

	for (Enemy* enemy : enemies) {
		if (enemy->IsDead()) {
			isLockOn_ = false;
			continue;
		}

		Vector3 enemyWorldPos = enemy->GetWorldPosition();
		Matrix4x4 matViewPort = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		Matrix4x4 matViewProjectionViewPort = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewPort);
		Vector3 enemyScreenPos = TransForm(matViewProjectionViewPort, enemyWorldPos);

		Vector2 reticlePos2D = Get2DReticlePosition();
		Vector2 enemyScreenPos2D = Vector2(enemyScreenPos.x, enemyScreenPos.y);

		float dis = float(Distance(enemyScreenPos2D, reticlePos2D));

		if (dis < lockOnRange) {
			isLockOn_ = true;
			// sprite2DReticle_->SetPosition(enemyScreenPos2D);
			sprite2DReticleLockOn_->SetPosition(enemyScreenPos2D);
			easingT_ = 0.0f;
			break;
		} else {
			if (isLockOn_) {
				while (easingT_ < 1.0f) {
					isLockOn_ = true;
					easingT_ += 0.01f;
					sprite2DReticleLockOn_->SetPosition(EaseIn(sprite2DReticleLockOn_->GetPosition(), reticlePos2D, easingT_));
					sprite2DReticle_->SetPosition(EaseIn(sprite2DReticle_->GetPosition(), reticlePos2D, easingT_));
				}
			}

			isLockOn_ = false;
		}
	}
}

void player::ReticleMultiLockOn(ViewProjection& viewProjection, std::list<Enemy*> enemies) {
	float lockOnRange = 30.0f;

	for (Enemy* enemy : enemies) {
		if (enemy->IsDead()) {
			continue;
		}

		Vector3 enemyWorldPos = enemy->GetWorldPosition();
		Matrix4x4 matViewPort = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		Matrix4x4 matViewProjectionViewPort = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewPort);
		Vector3 enemyScreenPos = TransForm(matViewProjectionViewPort, enemyWorldPos);

		Vector2 reticlePos2D = Get2DReticlePosition();
		Vector2 enemyScreenPos2D = Vector2(enemyScreenPos.x, enemyScreenPos.y);

		float dis = float(Distance(enemyScreenPos2D, reticlePos2D));

		if (dis < lockOnRange && enemy->GetIsLocked() == false) {
			enemy->SetLockOn(true);
		}

	}
}


void player::OnCollision() {}