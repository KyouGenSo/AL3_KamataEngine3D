#include"RailCamera.h"

void RailCamera::Initialize(Vector3 translation, Vector3 rotation) {

	worldTransform_.translation_ = translation;
	worldTransform_.rotation_ = rotation;

	worldTransform_.Initialize();
	viewProjection_.Initialize(); 
}

void RailCamera::Update(std::vector<Vector3> controlPoints) {

	ImGui::Begin("Rail Camera");
	ImGui::SliderFloat3("Translation", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotation", &worldTransform_.rotation_.x, -3.14f, 3.14f);
	ImGui::End();

	Vector3 currentPos = CatmullRomPosition(controlPoints, t_);
	SetTranslation(currentPos);

	// レールカメラ回転角度計算
	Vector3 target = CatmullRomPosition(controlPoints, t_ + 0.005f);
	Vector3 forward = Normalize(target - currentPos);

	float horizontalDistance = std::sqrt(forward.x * forward.x + forward.z * forward.z);

	SetRotation(Vector3(-std::atan2(forward.y, horizontalDistance), std::atan2(forward.x, forward.z), 0.0f));

	t_ += 0.001f;

	if (t_ >= 1.0f) {
		t_ = 0.0f;
	}

	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

}

void RailCamera::Draw() {

}

