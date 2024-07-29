#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*> models) {
	models_ = models;

	worldTransform_.Initialize();
}

void BaseCharacter::Update() {
	worldTransform_.TransferMatrix();
}

void BaseCharacter::Draw(const ViewProjection& viewProjection) {
	for (auto& model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}