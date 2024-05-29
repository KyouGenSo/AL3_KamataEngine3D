#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include <cassert>

class Skydome {
public:
	Skydome();
	~Skydome();

	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
};