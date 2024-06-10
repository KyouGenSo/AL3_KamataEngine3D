#include "player.h"

player::player() {}

player::~player() {
	// プレイヤーの弾の解放
	for (playerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
	delete model_;
	delete model3DReticle_;
}

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

	input_ = Input::GetInstance();

	worldTransform_.Initialize();

	worldTransform_.translation_ = pos;

	worldTransform_.UpdateMatrix();
}

void player::Update(ViewProjection& viewProjection) {

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
	Attack();

	// プレイヤーの弾の更新
	for (playerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// アフィン変換行列の作成
	worldTransform_.UpdateMatrix();

	// 3Dレティクルの更新
	Update3DReticle(viewProjection);

	// ImGui
	ImGui::Begin("Player Pos");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, -1.0f, 1.0f);
	ImGui::Text("x: %f, y: %f, z: %f", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
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

void player::DrawUI() { sprite2DReticle_->Draw(); }

void player::Rotate() {
	const float kRotateSpeed = 0.02f;

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotateSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotateSpeed;
	}
}

void player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 bulletVelocity = {0.0f, 0.0f, 0.0f};

		Vector3 diff = Subtract(GetWorldPosition3DReticle(), GetWorldPosition());
		diff = Normalize(diff);
		bulletVelocity = Multiply(diff, kBulletSpeed);

		Vector3 playerPos = GetWorldPosition();

		playerBullet* newBullet_ = new playerBullet();
		newBullet_->Initialize(model_, playerPos, bulletVelocity);

		bullets_.push_back(newBullet_);
	}
}

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

void player::OnCollision() {}

void player::Update3DReticle(ViewProjection& viewProjection) {
	// 3Dレティクルのワールド座標変換
	const float kReticleDistance = 20.0f;
	Vector3 offset = {0.0f, 0.0f, 1.0f};

	offset = TransFormNormal(offset, worldTransform_.matWorld_);
	offset = Multiply(Normalize(offset), kReticleDistance);
	worldTransform3DReticle_.translation_ = Add(worldTransform_.translation_, offset);
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標から2Dスクリーン座標への変換
	Vector3 reticlePos = GetWorldPosition3DReticle();
	Matrix4x4 matViewPort = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProjectionViewPort = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewPort);
	reticlePos = TransForm(matViewProjectionViewPort, reticlePos);
	sprite2DReticle_->SetPosition(Vector2(reticlePos.x, reticlePos.y));
}

void player::Update2DReticle(ViewProjection& viewProjection) {
	// マウス座標（スクリーン座標）を取得
	POINT mousePos;
	GetCursorPos(&mousePos);
	HWND hWnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hWnd, &mousePos);

	Matrix4x4 matViewPort = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewPort);
	Matrix4x4 inverseMatVPV = Inverse(matVPV);
}

void player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }