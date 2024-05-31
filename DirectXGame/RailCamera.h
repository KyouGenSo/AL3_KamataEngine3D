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

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};