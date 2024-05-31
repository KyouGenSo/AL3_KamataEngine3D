#include"RailCamera.h"

void RailCamera::Initialize(Vector3 translation, Vector3 rotation) {

	worldTransform_.translation_ = translation;
	worldTransform_.rotation_ = rotation;

	worldTransform_.Initialize();
	viewProjection_.Initialize(); 
}

void RailCamera::Update() {

	ImGui::Begin("Rail Camera");
	ImGui::SliderFloat3("Translation", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotation", &worldTransform_.rotation_.x, -3.14f, 3.14f);
	ImGui::End();

	//worldTransform_.rotation_.y += 0.01f;
	worldTransform_.translation_.z -= 0.1f;
	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

}

void RailCamera::Draw() {

}
