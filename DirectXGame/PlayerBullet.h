#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Matrix4x4Function.h"
#include <cassert>
#include "ImGuiManager.h"
#include "myFunction.h"
#include <algorithm>
#include "TextureManager.h"

class playerBullet {
public:
	playerBullet();
	~playerBullet();

	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

};

