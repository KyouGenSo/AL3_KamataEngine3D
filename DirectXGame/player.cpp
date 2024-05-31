#include "player.h"

player::player() {}

player::~player() {
	// プレイヤーの弾の解放
	for (playerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {

	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();

	worldTransform_.Initialize();

	worldTransform_.translation_ = pos;

	worldTransform_.UpdateMatrix();
}

void player::Update() {

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

	// ImGui
	ImGui::Begin("Player Pos");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, -1.0f, 1.0f);
	ImGui::Text("x: %f, y: %f, z: %f", worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	ImGui::End();
}

void player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// プレイヤーの弾の描画
	for (playerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

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
		Vector3 bulletVelocity = {0.0f, 0.0f, kBulletSpeed};

		bulletVelocity = TransFormNormal(bulletVelocity, worldTransform_.matWorld_);

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

Vector3 player::GetWorldRotation() {
	Vector3 worldRot;
	worldRot.x = worldTransform_.rotation_.x;
	worldRot.y = worldTransform_.rotation_.y;
	worldRot.z = worldTransform_.rotation_.z;

	return worldRot;
}


void player::OnCollision() {}

void player::SetParent(const WorldTransform* parent) {
	worldTransform_.parent_ = parent;
}