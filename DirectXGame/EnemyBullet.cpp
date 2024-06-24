#include "EnemyBullet.h"
#include "player.h"

EnemyBullet::EnemyBullet(){}

EnemyBullet::~EnemyBullet(){}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) { 
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("./Resources/red.png");

	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3(0.5f, 0.5f, 3.0f);

	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);

	// 解法1
	Matrix4x4 thetaYRotationMatrix = MakeRotateMatrixY(std::atan2(velocity.y, velocity.z));
	Vector3 velocityZ = TransForm(thetaYRotationMatrix, velocity);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	// 解法2
	//Vector3 velocityXZ = Vector3(velocity.x, 0.0f, velocity.z);
	//float XZlength = (float)Length(velocityXZ);
	//worldTransform_.rotation_.x = std::atan2(-velocity.y, XZlength);

	worldTransform_.translation_ = position;

	worldTransform_.UpdateMatrix();

	velocity_ = velocity;
}

void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	float speed = 1.2f;

	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);

	velocity_ = Multiply(Slerp(Normalize(velocity_), Normalize(toPlayer), t), speed);

	worldTransform_.translation_ += velocity_;

	Matrix4x4 thetaYRotationMatrix = MakeRotateMatrixY(std::atan2(velocity_.y, velocity_.z));
	Vector3 velocityZ = TransForm(thetaYRotationMatrix, velocity_);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_,viewProjection,textureHandle_);
}

void EnemyBullet::OnCollision() {
	isDead_ = true;
}