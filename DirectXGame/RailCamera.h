#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vector3.h"
#include "Matrix4x4Function.h"
#include "Matrix4x4.h"
#include "ImGuiManager.h"

class RailCamera {
public:
	void Initialize(Vector3 translation, Vector3 rotation);
	void Update();
	void Draw();

	Matrix4x4 GetViewMatrix() { return viewProjection_.matView; }
	Matrix4x4 GetProjectionMatrix() { return viewProjection_.matProjection; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetWorldTransform(const WorldTransform& worldTransform) {
		worldTransform_.translation_ = worldTransform.translation_;
		worldTransform_.rotation_ = worldTransform.rotation_;
		worldTransform_.scale_ = worldTransform.scale_;
	}

	void SetTranslation(Vector3 translation) { worldTransform_.translation_ = translation; }
	void SetRotation(Vector3 rotation) { worldTransform_.rotation_ = rotation; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};