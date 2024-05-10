#include "player.h"

player::player() {}

player::~player() {}

void player::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();

	worldTransform_.Initialize();
}

void player::Update() { 

	// 旋回
	Rotate();

	// 移動
	Vector3 move = { 0.0f, 0.0f, 0.0f };
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


	// アフィン変換行列の作成
	worldTransform_.UpdateMatrix();

	// 攻撃
	Attack();

	// プレイヤーの弾の更新
	if (bullet_) {
		bullet_->Update();
	}


	// ImGui
	ImGui::Begin("Player Pos");
	float playerPos[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};
	ImGui::SliderFloat3("playerPosition", playerPos, -35.0f, 35.0f);
	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];
	ImGui::Text("playerPosition: (%f, %f, %f)", playerPos[0], playerPos[1], playerPos[2]);
	ImGui::End();

}

void player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// プレイヤーの弾の描画
	if (bullet_) {
		bullet_->Draw(viewProjection);
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
		playerBullet* newBullet_ = new playerBullet();
		newBullet_->Initialize(model_, worldTransform_.translation_);

		bullet_ = newBullet_;

	}
}