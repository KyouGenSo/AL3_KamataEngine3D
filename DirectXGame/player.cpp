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
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// ImGui
	ImGui::Begin("Player Pos");
	float playerPos[3] = { worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z };
	ImGui::SliderFloat3("playerPosition", playerPos, -35.0f, 35.0f);
	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];
	ImGui::Text("playerPosition: (%f, %f, %f)", playerPos[0], playerPos[1], playerPos[2]);
	ImGui::End();


}

void player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}